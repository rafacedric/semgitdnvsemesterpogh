import argparse
import ssl
import random
import logging
from http.server import HTTPServer
from .users import UserType
from .desk_manager import DeskManager
from .simple_rest_server import SimpleRESTServer

logger = logging.getLogger("wifi2ble.main")


def setup_logging(log_level):
    numeric_level = getattr(logging, log_level.upper(), None)
    if not isinstance(numeric_level, int):
        raise ValueError(f"Invalid log level: {log_level}")
    logging.basicConfig(
        level=numeric_level,
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    )
    logger.info(f"Logging initialized at {log_level} level.")


def generate_desk_id():
    return ":".join(f"{random.randint(0, 255):02x}" for _ in range(6))


def generate_desk_name():
    return f"DESK {random.randint(1000, 9999)}"


def run(
        server_class=HTTPServer,
        handler_class=SimpleRESTServer,
        port=8000,
        use_https=False,
        cert_file=None,
        key_file=None,
        desks=2,
        speed=60,
        log_level="INFO",
):
    """
    Start the wifi2ble desk simulator.
    This function is safe to call from Django.
    """
    setup_logging(log_level)

    logger.info(f"Initializing DeskManager with simulation speed: {speed}")
    desk_manager = DeskManager(speed)

    logger.info("Adding default desks...")
    desk_manager.add_desk(
        "cd:fb:1a:53:fb:e6", "DESK 4486", "Desk-O-Matic Co.", UserType.ACTIVE
    )
    desk_manager.add_desk(
        "ee:62:5b:b8:73:1d", "DESK 6743", "Desk-O-Matic Co.", UserType.STANDING
    )

    current_count = len(desk_manager.get_desk_ids())
    if current_count < desks:
        logger.info(f"Adding {desks - current_count} additional desks.")
        for _ in range(desks - current_count):
            desk_manager.add_desk(
                generate_desk_id(),
                generate_desk_name(),
                "Desk-O-Matic Co.",
                UserType.ACTIVE,
            )

    desk_manager.start_updates()

    # Create handler with closure over desk_manager
    def handler(*args, **kwargs):
        return handler_class(desk_manager, *args, **kwargs)

    server_address = ("", port)  # '' = all interfaces
    SimpleRESTServer.initialize_api_keys()
    httpd = server_class(server_address, handler)

    if use_https:
        if not cert_file or not key_file:
            logger.error("Both certificate and key files must be provided for HTTPS.")
            raise ValueError("Both certificate and key files must be provided for HTTPS.")
        context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        context.load_cert_chain(certfile=cert_file, keyfile=key_file)
        httpd.socket = context.wrap_socket(httpd.socket, server_side=True)
        protocol = "HTTPS"
    else:
        protocol = "HTTP"

    logger.info(f"Starting {protocol} server on port {port}...")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        logger.info("Shutting down server...")
    finally:
        desk_manager.stop_updates()
        logger.info("Server stopped.")


# ----------------------------------------------------------------------
# CLI entry point – only runs when executed directly
# ----------------------------------------------------------------------
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="wifi2ble Desk Simulator")
    parser.add_argument("--port", type=int, default=8000, help="Port (default: 8000)")
    parser.add_argument("--https", action="store_true", help="Enable HTTPS")
    parser.add_argument("--certfile", type=str, help="SSL cert file")
    parser.add_argument("--keyfile", type=str, help="SSL key file")
    parser.add_argument("--desks", type=int, default=2, help="Number of desks (default: 2)")
    parser.add_argument("--speed", type=int, default=60, help="Simulation speed (default: 60)")
    parser.add_argument("--log-level", type=str, default="INFO", help="Log level")

    args = parser.parse_args()

    run(
        port=args.port,
        use_https=args.https,
        cert_file=args.certfile,
        key_file=args.keyfile,
        desks=args.desks,
        speed=args.speed,
        log_level=args.log_level,
    )
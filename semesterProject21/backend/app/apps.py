# app/apps.py
import threading
import logging
import os
import sys

from django.apps import AppConfig
from django.conf import settings

logger = logging.getLogger("wifi2ble")


class AppConfig(AppConfig):
    default_auto_field = 'django.db.models.BigAutoField'
    name = 'app'

    def ready(self):
        if os.environ.get('RUN_MAIN') != 'true':
            return
        if any(arg in sys.argv for arg in ['migrate', 'makemigrations', 'test', 'collectstatic']):
            return
        if not getattr(settings, 'WIFI2BLE_AUTO_START', False):
            return

        try:
            from wifi2ble.simulator.main import run
        except Exception as e:
            logger.error(f"[wifi2ble] Import failed: {e}")
            return

        thread = threading.Thread(
            target=run,
            kwargs={
                'port': settings.WIFI2BLE_PORT,
                'desks': settings.WIFI2BLE_DESKS,
                'speed': settings.WIFI2BLE_SPEED,
                'log_level': settings.WIFI2BLE_LOG_LEVEL,
            },
            daemon=True,
            name='wifi2ble-simulator',
        )
        thread.start()

        logger.info(
            f"[wifi2ble] AUTO-STARTED → {settings.WIFI2BLE_DESKS} desks @ http://0.0.0.0:{settings.WIFI2BLE_PORT}"
        )
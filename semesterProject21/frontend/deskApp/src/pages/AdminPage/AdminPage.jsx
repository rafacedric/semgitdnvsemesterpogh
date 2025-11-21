import { Outlet } from "react-router";

export default function AdminPage({isAdmin}) {
    return (
        <>
            {/* <title>Admin Dashboard</title> */}
            <main className="flex flex-col items-center justify-center min-h-screen w-full">
                <Outlet />
            </main>
        </>
    )
}
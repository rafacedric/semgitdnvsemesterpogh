import LoginPage from './pages/LoginPage/LoginPage';
import { BrowserRouter, Routes, Route } from "react-router";
import AdminPage from './pages/AdminPage/AdminPage';
import ManageUsersPage from './pages/AdminPage/ManageUsersPage';
import DashboardHomePage from './pages/DashboardHomePage';
import CreateUserPage from './pages/AdminPage/CreateUserPage';
import SingleUserPage from './pages/AdminPage/SingleUserPage';
import './App.css'

function App() {
  return (
    <>
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<LoginPage />}/>
          <Route path="/manage/*" element={<AdminPage isAdmin={true} />}>
              <Route index element={<DashboardHomePage isAdmin={true}/>} />
              <Route path="users" element={<ManageUsersPage />}/>
              <Route path="users/:user_id" element={<SingleUserPage />}/>
              <Route path="users/create" element={<CreateUserPage />}/>
          </Route>
          <Route path="/:username" element={<DashboardHomePage isAdmin={false} />}/>
      </Routes>
    </BrowserRouter>
    </>
  )
}

export default App

import { useNavigate } from "react-router";
import axios from "axios";
import { useEffect, useState } from "react";
import { Link } from "react-router";
import UserInfoDisplay from "../../components/UserInfoDisplay";

function getUsers() {
  const baseUrl = "http://localhost:8000";
  return axios(`${baseUrl}/api/users/`);
}

export default function ManageUsersPage() {
  // const users = getUsers();
  const [users, setUsers] = useState([]);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    async function fetchUsers() {
      try {
        const response = await getUsers();
        setUsers(response.data);
      } catch (error) {
        console.log("Failed to fetch users", error);
      } finally {
        setLoading(false);
      }
    }

    fetchUsers();
  }, []);

  const navigate = useNavigate();
  return (
    <>
      {/* No users to display */}

      {/* {users.length === 0 && 'No Users to display'} */}

      {!loading && users && users.length > 0 && <h1> All users </h1>}

      {loading
        ? "Loading users... Wait a second"
        : users && users.length > 0
        ? users.map((user) => (
            <Link key={user.id} to={`/manage/users/${user.id}`}>
              <UserInfoDisplay>{user.username}</UserInfoDisplay>
            </Link>
          ))
        : "No Users to display"}

      {!loading && (
        <button
          className="rounded-full text-white font-extralight bg-[#33A95E] w-60 p-2 m-4"
          onClick={() => navigate("/manage/users/create")}
        >
          Add a new user
        </button>
      )}
    </>
  );
}

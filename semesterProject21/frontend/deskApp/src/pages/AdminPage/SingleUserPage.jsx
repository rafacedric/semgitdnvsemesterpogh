import { useParams } from "react-router";
import axios from "axios";
import { useQuery } from "@tanstack/react-query";
import UserInfoDisplay from "../../components/UserInfoDisplay";

// fetching user data
function getUserData(user_id) {
  const baseUrl = "http://localhost:8000";
  return axios
    .get(`${baseUrl}/api/users/${user_id}`)
    .then((response) => response.data);
}

// rendering single user page
export default function SingleUserPage() {
  const { user_id } = useParams();

  // fetching data from data base
  const {
    data: user,
    isLoading,
    error,
  } = useQuery({
    queryKey: ["user", user_id],
    queryFn: () => getUserData(user_id),
    enabled: !!user_id,
  });

  // displaying errors and other states
  if (isLoading) return <div>Loading ...</div>;
  if (error) return <div>{error.message}</div>;
  if (!user) return <div>No user found </div>;

  return (
      <>
          <h1 className="text-[#33A95E] text-3xl font-light m-4">{user.username}</h1>
          <UserInfoDisplay>Current user's table</UserInfoDisplay>
          <UserInfoDisplay>User Id: {user_id}</UserInfoDisplay>
          <UserInfoDisplay>Username: {user.username}</UserInfoDisplay>

          {/* rendering full name only if the full name is not empty */}
          {user.first_name && user.last_name ? <UserInfoDisplay>Full Name: {user.first_name} {user.last_name} </UserInfoDisplay> : null}
    </>
  );
}

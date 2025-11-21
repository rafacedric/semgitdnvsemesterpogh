import { Link, useNavigate, useParams } from "react-router";
import LoginInputField from "../components/InputField";

const buttonsStyle = "border rounded-md p-2 bg-[#33A95E] hover:bg-green-700 text-white"


function ButtonsArea() {
  const navigate = useNavigate();

  return (
    <>
      <div className="button-container flex flex-col gap-6 max-w-md mx-auto min-w-[300px]">
        <button
          className={buttonsStyle}
          onClick={() => navigate("/manage/users")}
        >
          {" "}
          Manage Users
        </button>
        <button className={buttonsStyle}> Users Activity </button>
      </div>
    </>
  );
}

export default function DashboardHomePage({ isAdmin }) {
  const { username } = useParams();

  return (
    <>
      <h1 className="p-5" >
        {" "}
        {isAdmin ? "Welcome, admin!" : `Welcome, ${username}`}{" "}
      </h1>
      {isAdmin && <ButtonsArea />}
    </>
  );
}

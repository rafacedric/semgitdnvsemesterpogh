import axios from 'axios'
import { useState } from 'react'
import { useNavigate } from "react-router";
import InputField from '../../components/InputField';

function LoginForm() {
    
    const baseUrl = "http://localhost:8000"
    const [username, setUsername] = useState("")
    const [password, setPassword] = useState("")
    const navigate = useNavigate();

    const handleSubmit = (e) => {
        e.preventDefault()
        
        const user = {
            username, 
            password
        }

        axios.post(`${baseUrl}/api/login/`, user).then((response) => {
            console.log("User logged in to the basis successfully")

            const data = response.data

            console.log("Full response:", data);  // Add this
            console.log("data.admin:", data.admin);
            
            // showing page for an admin
            if (data.admin) {
                navigate('/manage')
            } else { navigate(`/${data.username}/`); }
            
        }).catch((error) => {
            console.log(error)
        });
    }

    return (
        <>
            <div className='flex flex-col justify-center items-center min-h-screen gap-8'>
                <h1>Login to my platform!</h1>
                <form onSubmit={handleSubmit} className="flex flex-col p-3 md:max-w-[300px] gap-8 items-center">
                    <InputField type="text" value={username} onChange={(e) => setUsername(e.target.value)} name="username" id="usernameField" placeholder="Username" autoComplete="off"/>
                    <InputField type="password" value={password} onChange={(e) => setPassword(e.target.value)} name="password" id="passwordField" placeholder="Password" autoComplete="off"/>
                    <button className="w-50 bg-black text-white p-4 rounded-full" type="submit" id="loginBtn" >Login</button>
                </form>
                <p>Forgot the password?</p>
                </div>
        </>
    );
}

// creating a form for a login page

export default function LoginPage(){
    return (
        <>
            <LoginForm />
        </>
    );
};
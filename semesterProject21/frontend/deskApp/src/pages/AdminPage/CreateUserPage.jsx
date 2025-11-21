/* a function for creating a new user */

import axios from 'axios'
import { useState } from 'react'
import InputField from '../../components/InputField'
import { useMutation, useQueryClient, QueryClient, QueryClientProvider } from '@tanstack/react-query'

const baseUrl = "http://localhost:8000";

const queryClient = new QueryClient();

const addUser = async (newUser) => {
    const response = await axios.post(`${baseUrl}/api/create/`, newUser);
    
    return response.data;
}

function UserForm() {
    const [username, setUsername] = useState("");
    const [password, setPassword] = useState("");
    const [email, setEmail] = useState("");
    const [firstName, setFirstName] = useState("");
    const [lastName, setLastName] = useState("");

    
    function clearForm() {
        setUsername("");
        setPassword("");
        setEmail("");
        setFirstName("");
        setLastName("");
    }

    const client = useQueryClient();

    const mutation = useMutation({
        mutationFn: addUser,
        onError: (error, newUser, context) => {
            console.error('Error occurred while adding a user: ', error);
            if (context?.existingUsers) { client.setQueryData(['users'], context.existingUsers) }
        },
        onSuccess: (data) => {
            // queryClient.invalidateQueries('users');
            clearForm();
        },
        onMutate: async (newUser) => {
            await client.cancelQueries(['users']);
            const existingUsers = client.getQueryData(['users']);
            client.setQueryData(['users'], (old) => {
                const tempUser = { id: Date.now(), ...newUser };
                return Array.isArray(old) ? [...old, tempUser] : [tempUser];
            })
            return { existingUsers }
        },
        onSettled: (data, error) => {
            client.invalidateQueries(['users']);
        }
    });

        const handleSubmit = (e) => {
        e.preventDefault()
        
        const new_user = {
            username, 
            password,
            email,
            firstName,
            lastName
        }

        mutation.mutate(new_user);
    }

    return (
        <>
            <h1>Add a new user</h1>

            {mutation.isError && (<div className='p-3 text-red-700 rounded-full text-light'>
                    Error occurred: {mutation.error.response?.data?.detail || mutation.error.message}
            </div>)}
            
            {mutation.isSuccess && (<div className='p-3 text-green-500 rounded-full text-light'>
                    User added successfully!
            </div>)}



            <form onSubmit={handleSubmit} method="post" className="flex flex-col gap-4 m-4">
                <InputField type="text" name="username" id="username" placeholder="Username" autoComplete="off" value={username} onChange={(e) => setUsername(e.target.value)}required/>
                <InputField type="password" name="password" id="password" placeholder="Password" autoComplete="off" value={password} onChange={(e) => setPassword(e.target.value)} required />
                <InputField type="text" name="firstName" id="firstName" placeholder="First name" autoComplete="off" value={firstName} onChange={(e) => setFirstName(e.target.value)} required />
                <InputField type="text" name="lastName" id="lastName" placeholder="Last name" autoComplete="off" value={lastName} onChange={(e) => setLastName(e.target.value)} required />
                <InputField type="email" name="userEmail" id="userEmail" placeholder="Email" autoComplete="off" value={email} onChange={(e) => setEmail(e.target.value)} required />
 
                {/* for future use if we decide to assign a table number */}

                {/* <label htmlFor="tableNumber"> Please select a table number </label>
                <select name="tableNumber" value={formData.tableNumber} onChange={handleChange} required>
                    <option value="" disabled>-- Select a table --</option>
                    <option value={1}>Table 1</option>
                    <option value={2}>Table 2</option>
                    <option value={3}>Table 3</option>
                </select>
                <label htmlFor="userRole" className=""> Please select a role </label>
                <select name="userRole" value={formData.userRole} onChange={handleChange} required>
                    <option value="" disabled>-- Select a role --</option>
                    <option value="admin">Administrator</option>
                    <option value="office_worker">Office employee</option>
                </select> */}

                <button type="submit" className='bg-black text-white p-4 rounded-full' disabled={mutation.isPending}>{mutation.isPending ? 'Adding...' : 'Add'}</button>
            </form>
        </>
    )
}
export default function CreateUserPage() {
    return (
        <>
            <QueryClientProvider client={queryClient}>
                <div className="flex flex-col justify-center min-h-screen items-center w-full align-center">
                    <UserForm />
                    </div>
            </QueryClientProvider>
        </>
    )
}
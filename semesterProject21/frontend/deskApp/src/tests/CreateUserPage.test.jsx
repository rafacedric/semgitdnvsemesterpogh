import { render, screen, fireEvent, waitFor } from '@testing-library/react';
import { describe, it, expect, vi, beforeEach } from 'vitest';
import axios from 'axios';
import CreateUserPage from '../pages/AdminPage/CreateUserPage';
import userEvent from '@testing-library/user-event';

// Mock axios globally
vi.mock('axios');

describe('CreateUserPage', () => {
  beforeEach(() => {
    vi.clearAllMocks();
    // Reset mock implementation to default for each test
    axios.post.mockReset();
  });

  it('should submit form with all required fields filled', async () => {
    const user = userEvent.setup();
    // Setup success mock
    axios.post.mockImplementation(async () => {
        // 2. Introduce a promise that resolves after 100 milliseconds
        await new Promise((resolve) => setTimeout(resolve, 300)); 
        
        // 3. Return the resolved data after the delay
        return { data: { id: 1, username: 'testuser' } };
    });

    render(<CreateUserPage />);

    // Fill inputs
    const usernameInput = screen.getByPlaceholderText(/username/i);
    const passwordInput = screen.getByPlaceholderText(/password/i);
    const firstNameInput = screen.getByPlaceholderText(/first name/i);
    const lastNameInput = screen.getByPlaceholderText(/last name/i);
    const emailInput = screen.getByPlaceholderText(/email/i);

    await user.type(usernameInput, 'testuser');
    await user.type(passwordInput, 'password123');
    await user.type(firstNameInput, 'John');
    await user.type(lastNameInput, 'Doe');
    await user.type(emailInput, 'john@example.com');

    // Submit
    const submitButton = screen.getByRole('button', { name: /add/i });
    await user.click(submitButton);

    // Assert Loading State (Optional, but good practice)
    expect(screen.getByText(/adding.../i)).toBeInTheDocument();

    // Assert API Call
    await waitFor(() => {
      expect(axios.post).toHaveBeenCalledTimes(1);
      expect(axios.post).toHaveBeenCalledWith(
        "http://localhost:8000/api/create/", // Fixed URL to match component
        {
          username: 'testuser',
          password: 'password123',
          firstName: 'John', // Matches snake_case mapping in handleSubmit
          lastName: 'Doe',   // Matches snake_case mapping in handleSubmit
          email: 'john@example.com'
        }
      );
    });
  });

  it('should handle successful form submission', async () => {
    const user = userEvent.setup();
    axios.post.mockResolvedValue({ data: { success: true } });

    render(<CreateUserPage />);

    // Fill minimal required fields
    await user.type(screen.getByPlaceholderText(/username/i), "newuser");
    await user.type(screen.getByPlaceholderText(/password/i), "pass123");
    await user.type(screen.getByPlaceholderText(/first name/i), "Jane");
    await user.type(screen.getByPlaceholderText(/last name/i), "Smith");
    await user.type(screen.getByPlaceholderText(/email/i), "jane@example.com");

    await user.click(screen.getByRole("button", { name: /add/i }));

    // Verify UI Feedback instead of console.log
    await waitFor(() => {
      expect(screen.getByText(/User added successfully!/i)).toBeInTheDocument();
    });
  });

  it('should handle form submission error', async () => {
    const user = userEvent.setup();
    const errorMessage = "Network Error";
    // Mock a rejected promise to simulate failure
    axios.post.mockRejectedValue({
      message: errorMessage,
      response: { data: { detail: errorMessage } }
    });

    render(<CreateUserPage />);

    // Fill inputs
    await user.type(screen.getByPlaceholderText(/username/i), "failuser");
    await user.type(screen.getByPlaceholderText(/password/i), "failpass");
    await user.type(screen.getByPlaceholderText(/first name/i), "Fail");
    await user.type(screen.getByPlaceholderText(/last name/i), "User");
    await user.type(screen.getByPlaceholderText(/email/i), "fail@example.com");

    await user.click(screen.getByRole("button", { name: /add/i }));

    // Verify Error UI Feedback instead of console.log
    await waitFor(() => {
      expect(screen.getByText(/Error occurred:/i)).toBeInTheDocument();
    });
  });

  // ... (Keep existing input validation tests like "should update username..." as they were correct)
  
  it("should update username input value when user types", async () => {
    const user = userEvent.setup();
    render(<CreateUserPage />);
    const input = screen.getByPlaceholderText("Username");
    await user.type(input, "testuser");
    expect(input.value).toBe("testuser");
  });
  
  it("should prevent form submission if username is empty", async () => {
    render(<CreateUserPage />);
    const input = screen.getByPlaceholderText("Username");
    expect(input).toHaveAttribute("required");
  });

});
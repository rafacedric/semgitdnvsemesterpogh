import { defineConfig } from 'vitest/config';
import react from '@vitejs/plugin-react';

export default defineConfig({
  plugins: [react()],
  test: {
    environment: 'jsdom',
    globals: true,
    setupFiles: './src/tests/setupTests.ts',
    
    // FIX for 'in-process is not supported' error.
    // Revert to 'threads' (the default) and specifically limit resources
    // to avoid the original 'Timeout starting forks runner' error.
    pool: 'forks',
    poolTimeout: 30000,
    
    poolOptions: {
      threads: {
        // Limit the number of threads to 1 or 2 to reduce system load and avoid timeouts.
        maxThreads: 2, 
        minThreads: 1
      }
    }
  },
});
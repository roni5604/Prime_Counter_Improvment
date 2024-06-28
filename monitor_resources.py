import subprocess
import psutil
import time
import matplotlib.pyplot as plt
import matplotlib

# Set the backend to TkAgg
matplotlib.use('TkAgg')

def run_process(seed, num_of_numbers):
    # Setup plotting
    fig = plt.figure(figsize=(10, 5))
    fig.canvas.manager.set_window_title("Live usages of CPU and RAM")  # Set the window title
    plt.ion()  # Turn on interactive mode

    times, memory_usage, cpu_usage = [], [], []
    start_time = time.time()

    # Initial empty plot display
    plt.subplot(2, 1, 1)
    plt.plot(times, memory_usage, label='Memory Usage (MB)', color='red')
    plt.title('Memory Usage Over Time')
    plt.xlabel('Time (s)')
    plt.ylabel('Memory Usage (MB)')
    plt.legend()

    plt.subplot(2, 1, 2)
    plt.plot(times, cpu_usage, label='CPU Usage (%)', color='blue')
    plt.title('CPU Usage Over Time')
    plt.xlabel('Time (s)')
    plt.ylabel('CPU Usage (%)')
    plt.legend()

    plt.show()
    # plt.pause(1)  # Pause to ensure the graph is displayed

    # Start the randomGenerator subprocess
    cmd_gen = f'./randomGenerator {seed} {num_of_numbers}'
    process_gen = subprocess.Popen(cmd_gen, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    # Start the primeCounter subprocess and pipe the output of randomGenerator to it
    process_prime = subprocess.Popen('./new_primeCounter', shell=True, stdin=process_gen.stdout, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    ps_process_prime = psutil.Process(process_prime.pid)
    print(f"The command that runs: ./randomGenerator {seed} {num_of_numbers} | ./new_primeCounter")
    print("PrimeCounter PID:", process_prime.pid)

    process_gen.stdout.close()  # Allow randomGenerator to receive a SIGPIPE if primeCounter exits

    try:
        while True:
            if process_prime.poll() is not None:
                print("Process has completed.")
                break

            current_time = time.time() - start_time
            times.append(current_time)

            # Get specific memory usage of the subprocess
            memory_info = ps_process_prime.memory_info()
            memory_usage.append(memory_info.rss / 1024 ** 2)  # Convert bytes to MB

            # Get system-wide CPU usage
            cpu_usage.append(psutil.cpu_percent(interval=0.1))

            # Plot updates
            plt.clf()
            plt.subplot(2, 1, 1)
            plt.plot(times, memory_usage, label='Memory Usage (MB)', color='red')
            plt.title('Memory Usage Over Time')
            plt.xlabel('Time (s)')
            plt.ylabel('Memory Usage (MB)')
            plt.legend()

            plt.subplot(2, 1, 2)
            plt.plot(times, cpu_usage, label='CPU Usage (%)', color='blue')
            plt.title('CPU Usage Over Time')
            plt.xlabel('Time (s)')
            plt.ylabel('CPU Usage (%)')
            plt.legend()

            plt.pause(0.1)  # Short pause to update the plot every 100ms

    except psutil.NoSuchProcess:
        print("The subprocess might have finished before monitoring started.")

    finally:
        # Capture stdout and stderr from the process
        stdout, stderr = process_prime.communicate()
        if stdout:
            print(stdout.decode())
        if stderr:
            print(stderr.decode())

        # Ensure final plot is shown
        plt.ioff()  # Turn off interactive mode
        plt.clf()  # Clear the figure for final plotting

        # Plot final results
        plt.subplot(2, 1, 1)
        plt.plot(times, memory_usage, label='Memory Usage (MB)', color='red')
        plt.title('Final Memory Usage Over Time')
        plt.xlabel('Time (s)')
        plt.ylabel('Memory Usage (MB)')
        plt.legend()

        plt.subplot(2, 1, 2)
        plt.plot(times, cpu_usage, label='CPU Usage (%)', color='blue')
        plt.title('Final CPU Usage Over Time')
        plt.xlabel('Time (s)')
        plt.ylabel('CPU Usage (%)')
        plt.legend()

        plt.tight_layout()
        # Set the title
        plt.show()  # Display the final plot

if __name__ == '__main__':
    seed = input("Enter seed value: ")
    num_of_numbers = input("Enter number of numbers: ")

    try:
        seed = int(seed)
        num_of_numbers = int(num_of_numbers)
        run_process(seed, num_of_numbers)
    except ValueError:
        print("Please enter valid integers for seed and number of numbers.")

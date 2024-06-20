import subprocess
import time
import psutil

# Function to run a shell command
def run_command(command):
    print(f"Running command: {command}")
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return process

# Function to monitor the resource usage of a process
def monitor_process(process, interval=1.0, timeout=600):
    pid = process.pid
    p = psutil.Process(pid)
    memory_usage = []
    cpu_usage = []

    start_time = time.time()
    while process.poll() is None:
        if time.time() - start_time > timeout:
            print("Timeout reached, terminating process.")
            process.terminate()
            break

        mem_info = p.memory_info()
        cpu_percent = p.cpu_percent(interval=interval)
        memory_usage.append(mem_info.rss / (1024 * 1024))  # Convert to MB
        cpu_usage.append(cpu_percent)
        print(f"Memory Usage: {memory_usage[-1]:.2f} MB, CPU Usage: {cpu_percent:.2f}%")
        time.sleep(interval)

    return memory_usage, cpu_usage

# Main function to run the generator and counter, and monitor the counter process
def main():
    seed = 10
    num_numbers = 10000000
    generator_command = f"./randomGenerator {seed} {num_numbers}"
    counter_command = "./primeCounter"
    
    # Start the random number generator
    generator_process = run_command(generator_command)
    time.sleep(5)  # Ensure the generator has time to produce numbers
    
    # Start the prime counter
    counter_process = run_command(counter_command)
    memory_usage, cpu_usage = monitor_process(counter_process)
    
    # Capture the output of the prime counter
    counter_output, _ = counter_process.communicate()
    counter_output = counter_output.decode("utf-8").strip()
    
    # Print captured output
    print("\nPrime Counter Output:")
    print(counter_output)
    
    # Summarize resource usage
    max_memory_usage = max(memory_usage)
    average_cpu_usage = sum(cpu_usage) / len(cpu_usage)
    
    print(f"\nMax Memory Usage: {max_memory_usage:.2f} MB")
    print(f"Average CPU Usage: {average_cpu_usage:.2f}%")
    
    if max_memory_usage < 2.0:
        print("Memory usage is below 2MB: Test passed")
    else:
        print("Memory usage exceeds 2MB: Test failed")

if __name__ == "__main__":
    main()

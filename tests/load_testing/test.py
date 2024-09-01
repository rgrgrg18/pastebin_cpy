import subprocess
import json
import grpc
import server_pb2
import server_pb2_grpc
import time
from concurrent.futures import ThreadPoolExecutor
import sys

def print_progress_bar(iteration, total, length=50):
    percent = (iteration / total) * 100
    filled_length = int(length * iteration // total)
    bar = '█' * filled_length + '-' * (length - filled_length)
    sys.stdout.write(f'\r[{bar}] {percent:.2f}% Complete')
    sys.stdout.flush()

# List to store all test public keys
public_key_list = []

# List to store average response times
average_latencies_get = []
average_latencies_make = []
average_latencies_del = []

# load configuration from config.json
def load_config():
    with open('config.json', 'r') as f:
        return json.load(f)

# Load config
config = load_config()
server_adress = config['server_adress']
total_requests = config['total_requests']
max_concurrent_requests = config['max_concurrent_requests']
read_write_ratio = config['read_write_ratio']
total_iterations = config['total_iterations']


#launches GHz with a get request
def run_ghz_get_test(concurrent, requests, public_key):
    command = [
        "ghz", "--insecure",
        "--proto", "server.proto",
        "--call", "pastebinApi.getPaste",
        "-d", json.dumps({"public_key": public_key}),
        "-c", str(concurrent),
        "-n", str(requests),
        "--format", "json",
        server_adress
    ]

    try:
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode == 0:
            return json.loads(result.stdout)
        else:
            print(f"Error: {result.stderr}")
            return None
    except Exception as e:
        print(f"Exception: {e}")
        return None


# gets the average time based on the result of the work GHz
def get_average_latency(result):
    if result and 'average' in result:
        avg_latency_ms = float(result['average']) / 1000000
        return avg_latency_ms
    else:
        print("No valid result or 'average' field missing.")
        return None


# create new paste and puts the key in the shared buffer
def make_new_paste():
    stub = server_pb2_grpc.pastebinApiStub(grpc.insecure_channel(server_adress))
    request = server_pb2.newPasteArgs(user_id=777, text="test_text")

    start_time = time.time()

    try:
        response = stub.makeNewPaste(request)

        public_key = response.public_key
        public_key_list.append(public_key)

        end_time = time.time()
        average_latencies_make.append((end_time - start_time)*1000)

    except grpc.RpcError as e:
        end_time = time.time()
        average_latencies_make.append((end_time - start_time)*1000)

        print(f"gRPC Error: {e}")


# deletes all pastes created during the test
def cleanup():
    stub = server_pb2_grpc.pastebinApiStub(grpc.insecure_channel(server_adress))

    for i in range(len(public_key_list)):
        public_key = public_key_list[i]
        print_progress_bar(i + 1, len(public_key_list))
        start_time = time.time()
        request = server_pb2.delPasteArgs(public_key=public_key)
        try:
            stub.deletePaste(request)
            end_time = time.time()
            average_latencies_del.append((end_time - start_time)*1000)
        except grpc.RpcError as e:
            end_time = time.time()
            average_latencies_del.append((end_time - start_time)*1000)
            print(f"gRPC Error while deleting paste with public key {public_key}: {e}")


# The runner function that runs the make_new_paste and run_ghz_get_test functions in parallel
def runner(total_requests, max_concurrent_requests, read_write_ratio):
    num_write_requests = int((1 - read_write_ratio) * total_requests)
    num_read_requests = total_requests - num_write_requests

    with ThreadPoolExecutor(max_workers = 1) as executor:
        write_futures = [executor.submit(make_new_paste) for _ in range(num_write_requests)]

        while len(public_key_list) == 0:
            time.sleep(0.01)  # Wait until the first write completes and adds a public key

        public_key = public_key_list[-1]
        read_future = executor.submit(run_ghz_get_test, max_concurrent_requests, num_read_requests, public_key)

        result = read_future.result()
        average_latencies_get.append(get_average_latency(result))

        for future in write_futures:
            future.result()



if __name__ == "__main__":

    # Print test input
    print(f"\nTotal requests: {total_requests} \n"
          f"Write requests: {int((1 - read_write_ratio) * total_requests)} \n"
          f"Read requests: {total_requests - int((1 - read_write_ratio) * total_requests)} \n")

    for i in range(total_iterations):
        print_progress_bar(i + 1, total_iterations)
        runner(total_requests // total_iterations, max_concurrent_requests, read_write_ratio)
    print("\n\n Starting cleanup \n")
    cleanup()
    print("\n")


    # Print results
    print("\nAverage Make Latencies (in ms):")
    print(sum(average_latencies_make) / len(average_latencies_make) if len(average_latencies_make) != 0 else 0)

    print("\nAverage Get Latencies (in ms):")
    print(sum(average_latencies_get) / len(average_latencies_get) if len(average_latencies_get) != 0 else 0)

    print("\nAverage Del Latencies (in ms):")
    print(sum(average_latencies_del) / len(average_latencies_del) if len(average_latencies_del) != 0 else 0, "\n")

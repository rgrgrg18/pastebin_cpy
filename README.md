# Pastebin cpy

API for quick interaction with text files


## The main features of the application

``` The user can create a text post ```

``` Each post has a unique key. Therefore, any user can get the post data using the key ```

``` For each post, the owner can configure access settings by adding a password ```


## Application structure

![project_structure_image](https://github.com/user-attachments/assets/b528cf31-7565-49a7-8b8a-f4ad8da2198f)

* For security, we generate a pair of public and private keys for each paste using sequences in SQL, the private one is used inside the system, the public one is available to users

* AWS services are used to store post data, each paste takes up no more than 1MB data

* To quickly get the texts and info of pastes we use redis, where stored the most popular pastes

* The files are stored in .bin, we use base64 for translation

----

## How to Build and Run

### Using Docker

1. **Install Docker Compose**

2. **Navigate to the Docker client directory**  
   Go to [`docker/client/`](docker/client/) and run the following command:
   ```bash
   ./create.sh
   ```

3. **Run the Docker services**  
   Start the services without logs:
   ```bash
   ./up.sh no-logs
   ```

   > For detailed instructions on interacting with Docker, refer to the our [Docker Documentation](docs/Docker.md).

---

### Configuration

4. **Edit the configuration file**  
   Open the `config.h` file and adjust the parameters as needed, using the sample configuration as a reference.

   > You can choose any version of the configuration file based on the availability of individual PostgreSQL and Redis services.  
   Ensure AWS parameters are correctly specified.

5. **Set up AWS credentials**  
   Navigate to the `~/.aws/` directory (macOS / Linux) and create two files:

    - **Credentials** (`credentials`):
      ```bash
      [default]
      aws_access_key_id = <static_key_id>
      aws_secret_access_key = <secret_key>
      ```

    - **Config** (`config`):
      ```bash
      [default]
      region=<your_region>
      ```

   If the directory doesn’t exist, you can use the command:
   ```bash
   aws configure
   ```

   > For more details, see the [AWS Documentation](https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/setup-linux.html).

6. **Start a local Redis server (for tests)**  
   To run a Redis server in the background, execute:
   ```bash
   redis-server --daemonize yes
   ```

---

### Running the Program

7. **Use the provided scripts**  
   To interact with the project, refer to the available [scripts](scripts/) and their descriptions in [docs/Scripts.md](docs/Scripts.md).

8. **Assemble and start the server**  
   Run the following script to build the program, run the tests, and start the server:
   ```bash
   /usr/src/app/scripts/run.sh
   ```

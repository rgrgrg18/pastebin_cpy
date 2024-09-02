# Pastebin cpy

API for quick interaction with text files


## The main features of the application

``` The user can create a text post ```

``` Each post has a unique key. Therefore, any user can get the post data using the key ```

``` For each post, the owner can configure access settings by adding a password ```


## Application structure

* For security, we generate a pair of public and private keys for each paste using sequences in SQL, the private one is used inside the system, the public one is available to users

* AWS services are used to store post data, each paste takes up no more than 1MB data

* To quickly get the texts and info of pastes we use redis, where stored the most popular pastes

* The files are stored in .bin, we use base64 for translation

## How to build and run locally

* Copy our repository and after that open `config.h` and change the parameters according to the sample.

> You can use any version of the configuration file depending on the availability of individual postgresql and redis services. Be sure to specify the aws parameters.

* Install docker-compose


* Go to [`docker/client/`](docker/client/) and run `./create.sh`


* Run `./up.sh no-logs` (running without logs) 


* To work with aws, go to the directory `~/.aws/` (macOS / Linux) 


* make file credentials and fill it:


```bash
[default]
  aws_access_key_id = <static_key_id>
  aws_secret_access_key = <secret_key>
```

* make file config and fill it:

```bash
[default]
  region=<your_region>
```

Also, if you do not have such a directory, use the command `aws configure` in terminal

(if you want to see more detailed instructions, go to the website [aws](https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/setup-linux.html))

(in our project, we use yandex cloud, so the instructions for changing the make file are relevant for us [yandex_cloud](https://yandex.cloud/ru/docs/storage/tools/aws-sdk-cpp))

* Start the local redis server (temporary measure for tests) `redis-server --daemonize yes`


* Run `/usr/src/app/scrips/run.sh`, this will assemble the program run the tests and start the server
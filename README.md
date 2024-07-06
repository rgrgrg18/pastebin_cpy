# Pastebin cpy

A file sharing system based on the telegram API


## The main features of the application

``` The user can create a text post ```

``` Each post has a unique key. Therefore, any user can get the post data using the key ```

``` For each post, the owner can configure access settings by adding a password ```


## Application structure

* For security, we generate a pair of public and private keys for each paste using sequences in SQL, the private one is used inside the system, the public one is available to users

* AWS services are used to store post data, each paste takes up no more than 1MB data

* To quickly get the texts of pastes we use redis, where stored the most popular pastes

* To store paste settings and get them quickly, we use the configuration from PostgreSQL and LFU cache

* The files are stored in .bin, we use base64 for translation

## How to build and run locally

* Install all the necessary libraries: [aws-sdk-cpp](https://github.com/aws/aws-sdk-cpp), [tgbot-cpp](https://github.com/reo7sp/tgbot-cpp), [pqxx](https://github.com/jtv/libpqxx), [redis-plus-plus](https://github.com/sewenew/redis-plus-plus) and their dependencies

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


* Copy our repository and after that open `bot/config.h` and change the parameters according to the sample

* Set a redis size limit to use it as a LRU cache [guide](https://cndoc.github.io/redis-doc-cn/cn/topics/lru-cache.html)

* Change file CMakeLists.txt for yourself

* Create your buid directory and navigate there

* Build the project

```bash
cd <BUILD_DIR>
cmake <path-to-root-of-this-source-code> \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_INSTALL_PREFIX=<path-to-install> \

cmake --build . --config=Debug
cmake --install . --config=Debug
```






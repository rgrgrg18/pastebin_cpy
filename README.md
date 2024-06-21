# How to build and run locally

----

* Install all the necessary libraries: [aws-sdk-cpp](https://github.com/aws/aws-sdk-cpp), [tgbot-cpp](https://github.com/reo7sp/tgbot-cpp), [pqxx](https://github.com/jtv/libpqxx) and their dependencies

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

----

* Copy our repository and after that open `bot/config.h` and change the parameters according to the sample

* Change file CMakeLists.txt if necessary

* Create your buid directory and navigate there

* Build the project

```bash
cd <BUILD_DIR>
cmake <path-to-root-of-this-source-code> \
-DCMAKE_BUILD_TYPE=Debug \
-DCMAKE_INSTALL_PREFIX=<path-to-install> \
-DBUILD_ONLY="s3"
cmake --build . --config=Debug
cmake --install . --config=Debug
```






FROM ubuntu:22.04

WORKDIR /usr/src/app

COPY ./install_deps.sh .

RUN chmod +x install_deps.sh
RUN ./install_deps.sh
RUN rm install_deps.sh

EXPOSE 6379
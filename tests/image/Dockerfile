# Use the official Python image from the Docker Hub
FROM python:3.11-slim

# Set the working directory inside the container
WORKDIR /app

# Copy necessary files
COPY tests/load_testing/ ./load_testing
COPY tests/acceptance_testing/ ./acceptance_testing
COPY src/server/proto/server.proto ./load_testing
COPY src/server/proto/server.proto ./acceptance_testing
COPY tests/scripts/run.sh .

# Install required packages including Git, SSH server, and Go
RUN apt-get update && apt-get install -y \
    git \
    openssh-server \
    wget \
    ca-certificates \
    vim \
    unzip \
    && rm -rf /var/lib/apt/lists/* \
    && wget https://golang.org/dl/go1.21.1.linux-amd64.tar.gz \
    && tar -C /usr/local -xzf go1.21.1.linux-amd64.tar.gz \
    && rm go1.21.1.linux-amd64.tar.gz \
    && wget https://github.com/protocolbuffers/protobuf/releases/download/v21.12/protoc-21.12-linux-x86_64.zip \
    && unzip protoc-21.12-linux-x86_64.zip -d /usr/local \
    && rm protoc-21.12-linux-x86_64.zip

# Set up environment variables
ENV PATH="/usr/local/go/bin:${PATH}"
ENV GOPATH="/root/go"
ENV PATH="${GOPATH}/bin:${PATH}"

# Install ghz
RUN go install github.com/bojand/ghz/cmd/ghz@latest

# Install Python packages
RUN pip install --no-cache-dir grpcio grpcio-tools protobuf==5.27.2

EXPOSE 9999
EXPOSE 22
RUN echo "StrictHostKeyChecking=no" >> /etc/ssh/ssh_config
RUN mkdir /var/run/sshd
CMD ["/usr/sbin/sshd", "-D"]

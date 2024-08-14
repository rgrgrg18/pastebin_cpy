FROM ubuntu:22.04

WORKDIR /usr/src/app

COPY docker/image/install_deps.sh .
COPY CMakeLists.txt .
COPY src/ src/
COPY scripts/ scripts/

RUN chmod +x install_deps.sh && chmod +x scripts/*
RUN ./install_deps.sh
RUN rm install_deps.sh

EXPOSE 6379
EXPOSE 9999

# (no recomend for production)
EXPOSE 22
RUN echo "StrictHostKeyChecking=no" >> /etc/ssh/ssh_config
RUN mkdir /var/run/sshd
CMD ["/usr/sbin/sshd", "-D"]
FROM ubuntu:latest

# Install Siege
RUN apt-get update && apt-get install -y siege
WORKDIR /tmp

# Set the entrypoint to run Siege
ENTRYPOINT ["bash"]

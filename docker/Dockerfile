### Build/test container ###
# Define builder stage
FROM projects:base as builder

# Share work directory
COPY . /usr/src/project
WORKDIR /usr/src/project/build

# Build and test
RUN cmake ..
RUN make
RUN ctest --output-on_failure

# To build, run the commands:
#   docker build -f docker/base.Dockerfile -t projects:base . --no-cache
#   docker build -f docker/Dockerfile -t projects:latest . --no-cache --progress=plain

# To build coverage:
#   docker build -f docker/coverage.Dockerfile -t projects:coverage . --no-cache --progress=plain

# To run the latest variant of projects as echo_run, removing the container after it stops, 
# mapping your server’s port to a port on your loopback network interface (127.0.0.1)
#   docker run --rm -p 127.0.0.1:80:80 --name echo_run projects:latest

# To view verbose build output, add --progress=plain to the build command.

# To stop the container:
#   docker stop echo_run

### Deploy hcontainer ###
# Define deploy stage
FROM ubuntu:jammy as deploy

#Copy server output binary to "."
COPY --from=builder /usr/src/project/build/bin/server .
COPY --from=builder /usr/src/project/conf/server.conf .

# Copy static files -- change
COPY --from=builder /usr/src/project/public ./public
# COPY --from=builder /usr/src/project/folder2 ./folder2

# Expose some port(s)
EXPOSE 80

# Use ENTRYPOINT to specify the binary name
# Update with real server name
ENTRYPOINT ["./server"]

# Use CMD to specify arguments to ENTRYPOINT
# Update with real server args
CMD ["server.conf"]

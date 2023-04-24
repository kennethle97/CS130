### Build/test container ###
# Define builder stage
FROM projects:base as builder

# Share work directory
COPY . /usr/src/project
WORKDIR /usr/src/project/build

# Build and test
#not sure if these are needed will tes
RUN cmake ..
RUN make
RUN ctest --output-on_failure


# Generate coverage report
RUN cd /usr/src/project
RUN gcovr

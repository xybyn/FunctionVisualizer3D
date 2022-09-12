FROM ubuntu:latest

WORKDIR /app
#COPY ./cmake ./app/cmake
#COPY ./include ./app/include
#COPY ./lib ./app/lib
#COPY ./resources ./app/resources
#COPY ./src ./app/src
#COPY ./thirdparty ./app/thirdparty
#COPY ./CMakeLists.txt ./app/CMakeLists.txt
COPY . .
RUN apt-get update 
RUN apt-get install -y --no-install-recommends 
RUN apt-get install -y gcc 
RUN apt-get install -y g++
RUN apt-get install -y build-essential
RUN apt-get install -y cmake
RUN apt-get install -y libglew-dev
RUN apt-get install -y libassimp-dev
RUN apt-get install -y libglu1-mesa-dev freeglut3-dev mesa-common-dev
RUN apt-get install -y x11-apps
WORKDIR /app/build
RUN cmake ..
RUN cmake --build .

CMD ["./OpenGLProject"]

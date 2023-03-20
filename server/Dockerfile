#FROM scratch
#FROM bash
#COPY server /
#EXPOSE 5000

#CMD ["/server"]

FROM ubuntu
WORKDIR /app

RUN apt-get update
RUN apt-get install libpq5 -y
RUN apt-get install g++ -y 

#COPY ./client .
ADD ./source ./source

RUN ln  -s /usr/lib/x86_64-linux-gnu/libpq.so.5.14 /usr/lib/x86_64-linux-gnu/libpq.so
RUN g++ -L/usr/lib/x86_64-linux-gnu -o "server" ./source/server-main.cpp    -lpq


EXPOSE 5000


#Only the first time : even if postgres container is started,
#it need to finish the execution of init.sql before server start
# so will sleep before launching server

CMD sleep 15 && ./server  

#FROM scratch
FROM ubuntu
WORKDIR /app

RUN apt-get update
RUN apt-get install libpq5 -y
RUN apt-get install g++ -y 

#COPY ./client .
COPY ./source ./source


RUN ln  -s /usr/lib/x86_64-linux-gnu/libpq.so.5.14 /usr/lib/x86_64-linux-gnu/libpq.so
RUN g++ -L/usr/lib/x86_64-linux-gnu -o "client" ./source/client-main.cpp  -lpq

#RUN apt-get update && apt-get -y install g++ && apt-get -y install make 

#RUN make
#RUN make clean
#ENTRYPOINT [ "bash" ]
#CMD ["/usr/local/bin/bash"]

#echo client started
#CMD sleep 30 && echo endsleep && ./client
CMD sleep 99999999999
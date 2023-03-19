FROM postgres
WORKDIR /docker-entrypoint-initdb.d
#RUN mkdir -p /docker-entrypoint-initdb.d

#ADD ./postgres/init.sql ./docker-entrypoint-initdb.d
COPY ./init.sql .

ENTRYPOINT ["docker-entrypoint.sh"]
EXPOSE 5432
CMD ["postgres"]



FROM debian:bullseye
RUN apt-get update -y && apt-get upgrade -y
RUN apt-get install -y nginx vim
RUN chown -R www-data:www-data /var/www/html
RUN chmod -R 755 /var/www/html
COPY ./nginx.conf /etc/nginx/nginx.conf
COPY ./index.html /var/www/html/
RUN nginx
ENTRYPOINT [ "sleep", "infinity" ]

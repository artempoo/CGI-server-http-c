FROM debian:bookworm-slim

RUN apt update && apt install -y gcc make fcgiwrap spawn-fcgi && rm -rf /var/lib/apt/lists/*
RUN mkdir -p /var/www/html/cgi-bin

COPY main.c /tmp/main.c
RUN gcc -Wall -Wextra -o /var/www/html/cgi-bin/app /tmp/main.c
RUN chmod +x /var/www/html/cgi-bin/app

COPY pages/ /var/www/html/pages/

CMD ["spawn-fcgi", "-n", "-a", "0.0.0.0", "-p", "9000", "-F", "1", "--", "/usr/sbin/fcgiwrap"]

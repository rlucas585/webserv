ARG OS
ARG OS_RELEASE

From ${OS}:${OS_RELEASE}

ENV DEBIAN_FRONTEND=noninteractive

RUN ln -fs /usr/share/zoneinfo/Europe/Amsterdam /etc/localtime

RUN apt update && apt install -y build-essential cmake curl

COPY conf.env /

COPY ubuntu_tester.conf /nginx.conf

ADD ubuntu_tester_root nginx_root

COPY ubuntu_tester /
RUN chmod +x /ubuntu_tester

COPY ./entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT /entrypoint.sh

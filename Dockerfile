FROM devkitpro/devkitppc:latest

COPY docker/init.sh init.sh
RUN chmod +x init.sh
RUN ./init.sh

ENTRYPOINT ["/bin/sh", "/cleanrip/docker/build.sh"]

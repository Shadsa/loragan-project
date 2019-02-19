# [WIP] 
# docker run -it -p 1680:1680 --rm --name lora-server lora-server
#
FROM node:8
WORKDIR /server
COPY server.js /server/
RUN npm install dgram
CMD ["node","server.js"]

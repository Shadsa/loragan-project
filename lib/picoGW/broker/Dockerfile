FROM ubuntu:bionic

RUN apt-get update
RUN apt-get install -y build-essential


WORKDIR "/code"
COPY . .


RUN make -C picoGW_packet_forwarder

WORKDIR "/code/picoGW_packet_forwarder/lora_pkt_fwd/"

#EXPOSE 1680/udp

ENTRYPOINT ./lora_pkt_fwd >> /dev/null

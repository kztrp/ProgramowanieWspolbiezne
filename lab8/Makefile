all: udp_client udp_server ftp_client ftp_server

udp_server: udp_server.c
	gcc udp_server.c -o udp_server -lrt

udp_client: udp_client.c
	 gcc udp_client.c -o udp_client -lrt

ftp_server: ftp_server.c
	gcc ftp_server.c -o ftp_server -lrt

ftp_client: ftp_client.c
	 gcc ftp_client.c -o ftp_client -lrt

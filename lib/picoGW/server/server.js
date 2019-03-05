var PORT = 1680;
var HOST = '127.0.0.1';

var dgram = require('dgram');
var server = dgram.createSocket('udp4');

server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', function (message, remote) {
    var msg = message.toString();
    var str = msg.substring(12,message.length);
    
    try {
        var json = JSON.parse(str);
        var payload = json.rxpk[0].data;
        decode = Buffer.from(payload, 'base64').toString('ascii');
        console.log(decode);
    } catch (e) {
        ;
    }
    

    //console.log(remote.address + ':' + remote.port +' - ' + json);

});

server.bind(PORT, HOST);


Multiplayer Pong Program


Team & Contributions:
Crystal Agerton (cagerton, 83259933)
   - Handled client side code and most client-server 
     communications.
Isaac Men (imen, 63309595)
   - Handled debugging, tick cycle and helped with client-server 
     communications.
Richard But (butr, 83672910)
   - Modified and debugged the physics engine/gamestate.



Environment Info:
The client was tested on Google Chrome.
The server was written and tested using Visual Studio 2017.



Instructions:
Setting up the server:
   - Navigate to the server83259933 folder
   - Run pong_server.exe
   - If this does not work, open the pong_server.sln file with
     Visual Studio 2017 and run and build without debugging

Setting up the clients:
   - Navigate to the client83259933 folder
   - Open the pong_client.html file in Google Chrome
   - Enter 127.0.0.1 as the Server IP
   - Enter 8000 as the Server Port
   - Enter a nickname
   - Click the connect button
   - Use the arrow keys to move your paddle

Troubleshooting:
   - If the client is taking too long to connect...
       - If the client appears to be connecting to the server but 
         has the server shows no sign of connection, restart the 
         server and try again.
       - If the client appears to be connecting to the server and 
         the server shows it has made a connection, wait for the
         server to show it has disconnected and try again.
import socket
import sys

host = "localhost"
port = 9000


request = (
            "POST / HTTP/1.1\r\n"
            "Host: localhost:9000\r\n"
            "Connection: keep-alive\r\n"
            "Content-Type: text/plain\r\n"
            "Transfer-Encoding: chunked\r\n"
            "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36\r\n"
            "Accept: */*\r\n"
            "\r\n"
            "1448\r\n"
            "Lorem ipsum odor amet, consectetuer adipiscing elit. Consequat volutpat netus efficitur habitasse nibh pellentesque nunc pharetra. Leo dictumst maecenas metus dui habitant. Venenatis etiam posuere; orci ultrices adipiscing habitasse tortor dis. Amet ligula vestibulum nec sollicitudin tristique. Nascetur montes erat ornare varius consequat diam sem. Iaculis class varius sollicitudin eleifend parturient consequat nibh cubilia. Inceptos velit tempus lorem per; faucibus volutpat porttitor. Nunc pharetra rutrum sapien; massa taciti malesuada. Sem auctor semper per vestibulum praesent sociosqu vel gravida. Sodales habitant rutrum orci felis pretium augue. Nulla finibus condimentum ac penatibus purus vestibulum? Quisque laoreet lectus est; efficitur efficitur est suscipit. Est potenti imperdiet ut suspendisse felis mus ex sodales tempus. Hac tempus velit ornare lobortis tellus natoque netus. Suspendisse bibendum nibh himenaeos sed sapien? Morbi orci cubilia convallis rhoncus interdum dignissim. Enim ultricies rutrum in mus, nascetur magna nostra iaculis. Ad mattis praesent fringilla massa auctor ligula ante potenti. Nisl magna proin ultrices sodales nostra. Dictum vivamus natoque viverra risus fusce tristique. Finibus vitae ex primis quisque dignissim curabitur pellentesque. Donec vulputate cursus suscipit montes nisl tellus. Mollis cursus sapien sem in auctor. Ad justo rhoncus eu cras risus blandit vulputate est. Tempus natoque mus dui platea pellentesque potenti nam. Apraesent inceptos habitasse ultrices mi facilisis magnis vitae habitasse. Nullam dolor egestas id vehicula magna montes, suscipit laoreet. Dolor neque nisi habitasse fames, posuere aliquet. Amet porttitor fusce ligula non sem et penatibus. Dui eu ipsum elit convallis aptent nec quam mus. Torquent platea adipiscing molestie volutpat, venenatis lacus litora. Nisl pellentesque blandit quisque facilisi praesent erat. Mauris orci consequat euismod felis venenatis dictum amet primis. Posuere ut eu eros condimentum rutrum. Felis mattis libero phasellus, vel sagittis finibus lectus. Platea ligula porta in placerat velit congue; nibh metus habitant. Tempus amet tellus felis sit nostra posuere efficitur libero. Euismod tellus urna turpis praesent tortor ridiculus dis ut. Vestibulum elit commodo torquent blandit dis dictum ut venenatis luctus. Per convallis netus a semper ac consequat. Nibh justo nullam nibh lorem nisl magnis non. Sit turpis platea primis tempus nascetur tempor conubia turpis morbi. Vulputate sagittis tincidunt vivamus ante natoque himenaeos leo aliquam. Tortor netus iaculis odio; platea molestie id ornare. Magnis platea tristique lobortis velit blandit; pharetra ad potenti. Amet purus odio et non vulputate. Placerat dis nostra parturient ipsum erat finibus lobortis; cubilia suspendisse. Praesent gravida condimentum habitasse vehicula, dolor taciti montes. Metus volutpat nostra sit nostra ultricies diam. Hendrerit senectus porttitor platea non lectus aliquam. Dis fringilla et himenaeos lacinia sagittis pharetra sociosqu tempor. Cras viverra nisl sed tempor ullamcorper tellus. Nunc vulputate tempus lacus; accumsan bibendum rutrum malesuada. Erat vulputate augue himenaeos aliquam pretium class tellus. Fusce porta ipsum scelerisque vitae platea mollis mauris. Nisl hendrerit donec rutrum malesuada turpis ut suscipit quis. Sem purus sollicitudin orci massa cubilia; curabitur arcu mattis. Sit dolor augue etiam potenti finibus eu luctus. Euismod hac porta facilisi egestas vel vitae class. Phasellus leo sodales laoreet tortor scelerisque parturient. Hendrerit tempus aliquet pretium nostra imperdiet vulputate. Suscipit phasellus morbi ad ut fermentum suscipit senectus. Molestie scelerisque nostra arcu nec in consequat. Rutrum egestas torquent eros hac odio convallis netus dictumst. Laoreet id dolor volutpat inceptos vel. Metus lobortis sed quam cursus torquent bibendum. Condimentum imperdiet potenti tempor iaculis fermentum. Dui vel adipiscing nam per augue ridiculus hendrerit interdum turpis. Mus habitant diam hendrerit sit magnis aliquam fames imperdiet. Finibus iaculis platea dolor maecenas condimentum nam consequat. Ridiculus aliquet nam ex diam curae egestas leo viverra sodales. Faucibus odio ex feugiat fringilla mus lacinia; egestas fusce. Nullam ad placerat lorem vehicula ex nibh. Ac quis neque massa potenti torquent lacinia eros. Quis mi efficitur lacinia magna luctus dui. Id euismod nam aliquam mi vestibulum. Velit molestie posuere hac montes, facilisi venenatis sollicitudin aliquam? Dolor lorem senectus tempor sed non maecenas phasellus magnis. Laoreet vivamus morbi imperdiet commodo ex posuere iaculis. Sit nostra primis nibh himenaeos accumsan aptent aliquam lacus tempus. Mollis nullam primis sapien maecenas vivamus. Semper ex ante curabitur efficitur blandit facilisis vehicula mus. Feugiat ipsum lectus donec metus fusce laoreet parturient habitasse. Lacus fringilla litora condimentum cubilia posuere. Fringilla habitasse adipiscing aptent mollis neque placerat. Penatibus natoque velit sagittis, efficitur tellus congue. Feugiat ipsum blandit pretium, congue ante sit. Platea sem tincidunt lacinia donec malesuada dui eget senectus."
            "959\r\n"
            "Lorem ipsum odor amet, consectetuer adipiscing elit. Mauris per purus elit netus ultricies montes tristique ultrices pharetra. Quam tellus montes scelerisque sodales porttitor tellus litora blandit. Primis vehicula aliquet nostra nascetur consectetur varius. Convallis turpis pharetra molestie habitant dis accumsan etiam cras? Arcu eleifend quis gravida a sociosqu. Taciti gravida nullam nostra sem quis finibus integer. Nostra pharetra conubia nisi inceptos malesuada habitasse congue. Sagittis sodales metus integer neque auctor vel. Maximus nisi interdum consectetur; aenean ad dis condimentum tellus. Aliquet vel sed; augue neque ultricies nunc cursus in semper. Porttitor consequat leo senectus ex facilisi tristique nisl. Porttitor conubia eleifend odio curabitur praesent duis aenean pharetra facilisis. Nec aenean phasellus in lacus cursus amet; est tortor hac. Dui sed phasellus maecenas purus nibh urna. Massa varius dui commodo in metus; nostra pretium. Lacus quis posuere sem at mauris. Tristique pharetra nullam mauris sit bibendum auctor nunc. Maximus ligula curae habitasse consequat, phasellus commodo ullamcorper lobortis. Nam pellentesque cras aliquam proin litora. Dui malesuada auctor ligula gravida tempor aliquet maecenas. Tempus lorem et netus bibendum finibus dapibus pulvinar sem ac. Habitant tincidunt vivamus sapien; rutrum semper tortor habitasse cras. Condimentum libero taciti sollicitudin amet varius mollis. Eleifend neque dignissim eu feugiat leo semper. Phasellus magnis tempus eget condimentum maecenas integer quisque arcu. Convallis at purus eget commodo euismod risus turpis nisl. Luctus aptent nunc facilisi ridiculus dolor venenatis metus. Donec quis aliquam tortor viverra cursus eleifend libero. Sociosqu maximus sodales mattis libero facilisi. Maecenas et curae maximus malesuada conubia ultrices id facilisis. Adipiscing sollicitudin hendrerit nulla condimentum sociosqu consectetur justo. Nam sodales adipiscing nunc morbi mi lacus. Nulla elit ultrices aliquet sodales suscipit. Inceptos vitae semper natoque penatibus elit tempor. Semper enim purus adipiscing gravida pretium curabitur aptent. Venenatis porttitor eros maecenas ad massa pulvinar. Tristique sem aptent metus arcu sed placerat laoreet. Massa metus auctor netus erat quisque conubia libero. Semper dictum aliquet hac et orci luctus blandit. Sociosqu sagittis hac cras sociosqu arcu."
            "0\r\n"
)

print("\033[1;33m--------------------------------------------------")
# print(request)
print("\033[1;32m--------------------------------------------------")
with socket.create_connection((host, port)) as sock:
    sock.sendall(request.encode())  # Sending the request to the server
    response = sock.recv(4096)  # Receiving the response
    print(response.decode())  # Decoding and printing the response
print("--------------------------------------------------\033[0m")

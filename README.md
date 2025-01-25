# webserv

### host:
    - context: server
    - syntax: host 0.0.0.0;
    - multiple directives: no
    - multiple values: no
    - default: 0.0.0.0
    - required: no

### listen:
    - context: server
    - syntax: listen 8080 8081 8081;
    - multiple directives: yes
    - multiple values: yes
    - default: 8080
    - required: no

### server_name:
    - context: server
    - syntax: server_name domain1 domain2 domain2;
    - multiple directives: yes
    - multiple values: yes
    - default: -
    - required: no

### client_body_buffer_size:
    - context: server
    - syntax: client_body_buffer_size size;
    - multiple directives: no
    - multiple values: no
    - default: 8k
    - required: no

### root:
    - context: location
    - syntax: root path
    - multiple directives: no
    - multiple values: no
    - default: /
    - required: no

### index:
    - context: location
    - syntax: index file
    - multiple directives: yes
    - multiple values: yes
    - default: index.html
    - required: no

### allow_methods:
    - context: location
    - syntax: allow_methods METHOD METHOD METHOD
    - multiple directives: yes
    - multiple values: yes
    - default: GET POST DELETE
    - required: no

### error_page:
    - context: location
    - syntax: error_page code code path
    - multiple directives: yes
    - multiple values: yes
    - default: -
    - required: no

### autoindex:
    - context: location
    - syntax: autoindex on | off
    - multiple directives: no
    - multiple values: no
    - default: off
    - required: no

### return :
    - context: location
    - syntax: return url
    - multiple directives: yes
    - multiple values: no
    - default: -
    - required: no

### cgi_extension :
    - context: location
    - syntax: cgi_extension path
    - multiple directives: yes
    - multiple values: no
    - default: -
    - required: no

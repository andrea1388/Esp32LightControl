idf_component_register(SRCS 
"ota.cpp"
"main.cpp"
"wifi.cpp"
"mqtt.cpp"
"commands.cpp"
INCLUDE_DIRS ""
EMBED_TXTFILES ${project_dir}/certs/ca.crt
)
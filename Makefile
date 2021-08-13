# Used to setup the files

CONF_DIR=/etc/p2pd
LOG_DIR=/var/log/p2pd

USER=$(shell whoami)

all: setup
.PHONY: setup remove

setup: | $(CONF_DIR) $(LOG_DIR)

$(CONF_DIR):
	@ echo "\033[34m[+]\033[0m ENTERING THE SETUP INSTALLATION \033[34m[+]\033[0m"

ifeq ($(USER),root)
	@ echo "\033[1;33m[!]\033[0m Do not run as root!"
else 
	@ echo "\033[1;33m[!]\033[0m Creating configuration dir $(CONF_DIR) ..."
	@ sudo mkdir -p $(CONF_DIR)
	@ echo "\033[32m[+]\033[0m Done!"
	
	@ echo "\033[1;33m[!]\033[0m Setting up ownership of $(CONF_DIR) ..."
	@ sudo chown -R $(USER) $(CONF_DIR)
	@ echo "\033[32m[+]\033[0m Done!"
	
	@ echo "\033[1;33m[!]\033[0m Copying the configuration files to $(CONF_DIR) ..."
	@ cp conf/client.conf $(CONF_DIR)
	@ echo "\033[32m[+]\033[0m Done!"
endif

 $(LOG_DIR):
ifeq ($(USER),root)
	@ echo "\033[1;33m[!]\033[0m Do not run as root!"
else
	@ echo "\033[1;33m[!]\033[0m Creating log dir $(LOG_DIR) ..."
	@ sudo mkdir -p $(LOG_DIR)
	@ echo "\033[32m[+]\033[0m Done!"
	
	@ echo "\033[1;33m[!]\033[0m Setting up ownership of $(LOG_DIR) ..."
	@ sudo chown -R $(USER) $(LOG_DIR)
	@ echo "\033[32m[+]\033[0m Done!"
endif

remove:
	@ sudo $(RM) -r $(CONF_DIR) $(LOG_DIR)
	@ echo "\033[32m[+]\033[0m Removed successfully!"
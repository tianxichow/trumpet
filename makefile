

TARGETS = libtrumpet.a
LIBOBJS = $(patsubst %.c,%.o,$(SOURCE)) 
SOURCE = $(wildcard src/*.c)
OBJSDIR = .objs
CFLAG = -Wall -lrt

DEMO_SOURCE = $(wildcard demo/*.c)
DEMO_OBJS = $(patsubst %.c,%.o,$(DEMO_SOURCE))
DEMO_TARGETS = $(patsubst %.c,%,$(notdir $(DEMO_SOURCE)))

OFFSET=\033[41G
SUCCESS_COLOR=\033[1;34m
FAILURE_COLOR=\033[1;31m
RESET=\033[0m
CLEAR=\033[H\033[J


all: $(TARGETS) demo

%.o: %.c
	@if (gcc $(CFLAG) -c $< -o $@);then \
		echo -e "$@ $(OFFSET)$(SUCCESS_COLOR)[ ok ]$(RESET)";\
	else \
		echo -e "$@ $(OFFSET)$(FAILURE_COLOR)[ failed ]$(RESET)";\
		exit 1;\
	fi

$(TARGETS):$(LIBOBJS)
	@if (ar rcs $@ $^);then \
		echo -e "$@ $(OFFSET)$(SUCCESS_COLOR)[ ok ]$(RESET)";\
	else \
		echo -e "$@ $(OFFSET)$(FAILURE_COLOR)[ failed ]$(RESET)";\
		exit 1; \
	fi
	@mkdir $(OBJSDIR) -p
	@mv src/*.o $(OBJSDIR)/

test:FORCE
	@echo "test"

demo:FORCE
	@$(foreach DEMO_TARGET,$(DEMO_TARGETS),\
		if (gcc $(CFLAG) -c demo/$(DEMO_TARGET).c -o demo/$(DEMO_TARGET).o);then \
			echo -e "demo/$(DEMO_TARGET).o $(OFFSET)$(SUCCESS_COLOR)[ ok ]$(RESET)"; \
		else \
			echo -e "demo/$(DEMO_TARGET).o $(OFFSET)$(FAILURE_COLOR)[ failed ]$(RESET)";\
			exit 1; \
		fi; \
		if (gcc $(CFLAG) demo/$(DEMO_TARGET).o libtrumpet.a -o demo/$(DEMO_TARGET)); then \
			echo -e "demo/$(DEMO_TARGET) $(OFFSET)$(SUCCESS_COLOR)[ ok ]$(RESET)"; \
		else \
			echo -e "demo/$(DEMO_TARGET) $(OFFSET)$(FAILURE_COLOR)[ failed ]$(RESET)";\
			exit 1; \
		fi; \
	)

clean:
	rm -fr $(TARGETS) 
	rm -fr $(OBJSDIR)
	rm -fr demo/$(DEMO_TARGETS)
	rm -fr $(DEMO_OBJS)

FORCE:

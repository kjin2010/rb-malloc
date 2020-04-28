CFLAGS=-g -std=c99 -O0

TEST_EXTS = .in .ok
UTCS_ID ?= $(shell pwd | sed -e 's/.*_//')

MY_TESTS = ${addprefix ${UTCS_ID},${TEST_EXTS}}
SIG = ${firstword ${shell (ls ${MY_TESTS} > /dev/null 2>&1  && (cat ${MY_TESTS} | md5sum)) || echo "?" }}

POSSIBLE_TESTS = ${basename ${wildcard *${firstword ${TEST_EXTS}}}}
TESTS = ${sort ${POSSIBLE_TESTS}}
TEST_EXECS = ${addsuffix .exec,$(TESTS)}
TEST_OKS = ${addsuffix .ok,${TESTS}}
TEST_RESULTS = ${addsuffix .result,${TESTS}}
TEST_TARGETS = ${addsuffix .test,${TESTS}}
TEST_OUTS = ${addsuffix .out,${TESTS}}
TEST_RAWS = ${addsuffix .raw,${TESTS}}
TEST_DIFFS = ${addsuffix .diff,${TESTS}}
C_FILES = ${wildcard *.c}
O_C_FILES = ${subst .c,.o,${C_FILES}}
S_FILES = ${wildcard *.s}
O_S_FILES = ${subst .s,.o,${S_FILES}}

O_FILES=${O_C_FILES} ${O_S_FILES}

all : ${TEST_EXECS};

%.o : %.c Makefile
	gcc $(CFLAGS) -MD -c $*.c

%.o : %.in Makefile
	gcc -x c $(CFLAGS) -MD -c $*.in

%.o : %.s Makefile
	gcc $(CFLAGS) -c $*.s

$(TEST_EXECS) : %.exec : Makefile ${O_FILES} %.o
	-gcc -o $*.exec ${O_FILES} $*.o

$(TEST_RAWS) : %.raw : Makefile %.exec
	-((timeout 5 ./$*.exec  || echo timeout) | head -n 10000) > $*.raw

$(TEST_OUTS) : %.out : Makefile %.raw
	-cp $*.raw $*.out

${TEST_DIFFS} : %.diff : Makefile %.out %.ok
	-(diff -wBb $*.out $*.ok || true) > $*.diff 2> /dev/null

${TEST_RESULTS} : %.result : Makefile %.diff
	(test -z "`cat $*.diff`" && echo "pass" > $*.result) || echo "fail" > $*.result

${TEST_TARGETS} : %.test : Makefile %.result
	@echo "$* ... `cat $*.result`"

test: Makefile ${TEST_RESULTS} ${TEST_TARGETS} ;

clean :
	rm -f *.raw *.out *.d *.o *.diff *.result $(TEST_EXECS)

-include *.d


#####################################################################


sig:
	@echo ${SIG}

tests :
	@echo "$(TESTS)"

copy_student_tests:
ifeq ($(SIG),?)
	@echo "no tests"
else
ifeq ($(TEST_DIR),)
	@echo "no TEST_DIR"
else
	@for i in $(TEST_EXTS); do \
	    cp $(UTCS_ID)$$i $(TEST_DIR)/$(SIG)$$i; \
	done
endif
endif

copy_given_tests:
ifeq ($(TEST_DIR),)
	@echo "no TEST_DIR"
else
	@for i in $(TEST_EXTS); do \
	    cp t?$$i $(TEST_DIR); \
	done
endif

remove_tests:
	@for i in $(TEST_EXTS); do \
	    rm -rf *$$i; \
	done

copy_results:
ifeq ($(RESULTS_DIR),)
	@echo "no RESULTS_DIR"
else
	-cp *.result $(RESULTS_DIR)
	-cp *.ok $(RESULTS_DIR)
	-cp *.out $(RESULTS_DIR)
	-cp *.raw $(RESULTS_DIR)
endif

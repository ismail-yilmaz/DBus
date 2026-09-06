topic "Design: Pseudoblocking, Async Mechanics, and API Usage";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5+75 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[b83;* $$22,22#78EB85B566C7E078B53494D0FD53D992:subtitle]
[2 $$23,23#1BDAF836AD481778AD91861722C13AB2:toc]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s2;= Pseudoblocking, Async Mechanics, and API Usage&]
[s5; Choosing between synchronous and asynchronous communication 
dictates the entire architecture of a U`+`+ application. Understanding 
how the DBus engine ([C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ D
BusConnection]) implements these wait states at the transport 
layer is critical to preventing graphical user interface freezes 
and maintaining memory safety. The engine provides a highly deterministic 
routing model, but it requires a solid understanding of how control 
flow shifts between the network socket and the application`'s 
user space.&]
[s3; Table of Contents&]
[s23; &]
[s0; [^topic`:`/`/DBus`/srcdoc`/Design`_en`-us`#1^ 1. The Mechanics 
of Pseudoblocking]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Design`_en`-us`#2^ 2. The Reentrancy 
Threat and the EventLock Guard]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Design`_en`-us`#3^ 3. Asynchronous 
Event`-Driven Multiplexing]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Design`_en`-us`#4^ 4. Server Dispatch 
Architecture]&]
[s23; &]
[s3;:1: 1. The Mechanics of Pseudoblocking&]
[s5; In standard networking libraries, a synchronous read operation 
typically triggers a true kernel`-level block. The executing 
thread sleeps entirely, all local execution halts, and the thread 
cannot awaken until bytes arrive at the socket interface. Within 
the DBus package, a synchronous request using [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()][C  ]does not execute a true thread`-halting block. 
Instead, it enters a pseudoblocking state.&]
[s5; When you invoke a synchronous method call, the engine serializes 
your payload, transmits it across the wire, and records the outbound 
serial number as [C@5 callserial]. The calling thread then enters 
[C@5 Run()], the engine`'s internal pump: a loop that repeatedly 
drains the socket, deserializes incoming bytes, and yields control 
to the operating system between attempts via a [^topic`:`/`/Core`/src`/SocketWaitEvent`_en`-us`#SocketWaitEvent`:`:class^ S
ocketWaitEvent], rather than spinning continuously. Each wake 
cycle is bounded by a configurable [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WaitStep`(int`)^ W
aitStep()] interval, but in practice the thread resumes as soon 
as the socket actually has data, since the wait is a genuine 
OS`-level readiness notification rather than a fixed poll delay. 
The thread remains inside this pump, processing whatever traffic 
arrives, until it extracts a complete message whose reply field 
matches [C@5 callserial], at which point the message is stored 
and the loop terminates successfully. Any other complete message 
that arrives during this window (a broadcast signal, an incoming 
method call targeting your own service, or even a stray reply 
belonging to no outstanding request) is still fully parsed and 
routed through its normal handler; only the one specific reply 
you are waiting for causes the pump to exit.&]
[s5; Termination is not guaranteed to be successful. If Timeout() 
has been set to a finite value (sixty seconds by default) and 
that interval elapses with no new bytes arriving, the connection 
throws a DBusError carrying [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ C
ONNECTION`_TIMED`_OUT], unwinding back through [C@5 Run()] and 
leaving the connection in a failed state, queryable afterward 
via [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:IsError`(`)const^ I
sError()], [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:GetError`(`)const^ G
etError()], and [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:GetErrorDesc`(`)const^ G
etErrorDesc()]. Setting [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Timeout`(int`)^ T
imeout(Null)] instead disables this ceiling entirely, which is 
appropriate for operations whose completion genuinely depends 
on unpredictable external timing; the mechanics of doing so for 
the [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()] case are detailed in Section 3.&]
[s5; The term pseudoblocking is deliberately chosen over blocking: 
at no point does the operating system truly suspend the thread 
for the operation`'s full duration the way a raw blocking socket 
read would. The thread is, at every moment, either actively parsing 
a just`-arrived frame or sitting in an efficient, near`-zero`-CPU 
OS wait for the next one. What actually distinguishes this mode 
from the asynchronous one described in Section 3 is not efficiency 
(both modes rely on the identical SocketWaitEvent wait mechanism 
underneath) but ownership: in pseudoblocking mode, [C@5 Run()] retains 
full control of that wait`-and`-poll cycle internally, on the 
calling thread, for the entire duration of the operation, meaning 
nothing else can run on that thread until it resolves. In the 
asynchronous mode, that same cycle is handed to the application`'s 
own event loop instead.&]
[s3;:2: 2. The Reentrancy Threat and the EventLock Guard&]
[s5; The pseudoblocking architecture introduces a severe chronological 
danger known as reentrancy. Because the socket is actively being 
pumped to locate your specific reply packet, other incoming messages 
such as broadcast signals or remote method calls targeting your 
service are still being deserialized and dispatched in real`-time. 
If you have defined event hooks like [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenSignal^ W
henSignal] or [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenMethodCall^ W
henMethodCall], the [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ D
][C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ BusCo
nnection][C  ]engine will fire them while the main thread is supposedly 
blocked waiting for its reply.&]
[s5; If a developer attempts to execute a new, secondary synchronous 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()] from within one of these callbacks, the engine would 
traditionally spin up a second, nested socket pump bound to a 
new outbound serial. Because callserial is a single connection`-wide 
value, the moment this nested call is issued it overwrites the 
serial the outer pump is still searching for (the outer loop`'s 
original target is gone, permanently, whether or not its reply 
has arrived yet. If that reply happens to arrive while the nested 
pump is still running, it is fully parsed and then silently discarded, 
since it no longer matches the now`-overwritten [C@5 callserial]) 
effectively stolen by the inner loop. If it arrives afterward, 
it fares no better, since callserial is never restored to its 
original value once the nested call completes. Either way, the 
outer [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()] can no longer succeed. It will eventually unwind 
via [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Timeout`(int`)^ T
imeout()] (assuming a finite one is set, which is the default) 
rather than hang forever, but from the developer`'s perspective 
the operation has failed for reasons that have nothing to do 
with the actual request, which is a difficult class of bug to 
diagnose after the fact.&]
[s5; To prevent this, the engine fires user`-space callbacks ([C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenSignal^ W
henSignal] , [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenMethodCall^ W
henMethodCall], and the per`-rule callbacks registered through 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:AddMatch`(const String`&`,Event`)^ A
ddMatch()]) from inside an [C@5 EventLock]: a small RAII guard that 
sets a dispatching flag for the duration of the callback and 
restores its previous value on exit, correctly handling the case 
where a callback itself triggers further dispatch. [C@5 EventLock 
]itself does not reject anything; it only marks the state. The 
actual rejection happens explicitly, at the top of [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()], which checks that flag and returns false immediately 
rather than allowing the corrupting nested pump described above 
to ever begin.&]
[s5; A second, related form of the same threat exists outside of 
callbacks entirely. Because [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Connect`(const String`&`,bool`)^ C
onnect()], [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()], and [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()] all unconditionally clear the connection`'s internal 
queue and reassign its completion condition, invoking any of 
them while a previous invocation is still actively running (for 
instance, calling [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()] from unrelated code elsewhere in the event loop 
while an asynchronous [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()] on the same connection instance has not yet been told 
to stop) produces exactly the same class of corruption as the 
callback case, just without a callback being involved at all. 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:InProgress`(`)const^ I
nProgress()] exists to make this state observable, and all three 
of these entry points check it themselves: attempting to start 
a new primary operation while one is already in progress on that 
connection returns false immediately rather than silently destroying 
whichever operation was already running.&]
[s5; Neither guard prevents a developer from reacting to what a callback 
receives; only from starting a brand`-new primary request/response 
cycle while doing so. For the common case of responding to what 
just arrived (sending a reply via[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:SendReply`(const DBusMessage`&`,const DBusValueArray`&`)^  
SendReply()] or [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:SendError`(const DBusMessage`&`,const String`&`,const String`&`)^ S
endError()], broadcasting a signal via [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:BroadcastSignal`(const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ B
roadcastSignal()], or adjusting a subscription via [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:AddMatch`(const String`&`,Event`)^ A
ddMatch()] or [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:RemoveMatch`(const String`&`)^ R
emoveMatch()]) none of these functions touch the queue, the packet 
buffer, or the completion condition that [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()], [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Connect`(const String`&`,bool`)^ C
onnect()], and [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()] depend on; they append to a separate side`-band buffer 
instead, and rely on whichever operation is already driving the 
connection to flush it on its next cycle. All of them can be 
called directly from inside a dispatched callback with no special 
handling required. Only a genuinely new, independent synchronous 
exchange (one that has nothing to do with what triggered the 
callback) needs to be deferred, for instance via [@5 Ctrl`::PostCallback()] 
onto the main GUI thread, or issued instead on a second, separate 
DBusConnection instance opened for that purpose.&]
[s3;:3: 3. Asynchronous Event`-Driven Multiplexing&]
[s5; To achieve true non`-blocking behavior and prevent UI starvation, 
the engine must surrender the socket pumping responsibility to 
the application`'s primary event loop. By invoking the [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:NonBlocking`(bool`)^ N
onBlocking()] configuration, the engine prepares its internal 
state machine for deferred payload processing:  [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Connect`(const String`&`,bool`)^ C
onnect()], [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()], and [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()] all return immediately in this mode without pumping 
anything themselves, leaving the actual work to be driven externally.&]
[s5; Rather than owning the wait`-and`-poll cycle internally, the 
developer registers the connection with a [C^topic`:`/`/Core`/src`/SocketWaitEvent`_en`-us`#SocketWaitEvent`:`:class^ S
ocketWaitEvent] multiplexer using [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:AddTo`(SocketWaitEvent`&`)^ A
ddTo()], which adds the connection`'s socket along with the correct 
interest flags: read interest is always requested, and write interest 
is added automatically only when the side`-band buffer described 
in Section 2 currently holds data waiting to be sent, avoiding 
spurious wakeups when there is nothing to write. When the operating 
system reports activity, the application calls [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Do`(`)^ D
o()], which asserts that [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:NonBlocking`(bool`)^ N
onBlocking()] has been set and then drives exactly one cycle 
of the same internal pump described in Section 1: draining whatever 
is currently available on the socket, reconstructing complete 
frames, and firing the corresponding callbacks under the same 
[C@5 EventLock ]protection. Because control returns to the caller 
after each cycle rather than looping internally, the application`'s 
own event loop remains free to service other connections, timers, 
or GUI events between calls to [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Do`(`)^ D
o()][C ;] this, not raw CPU efficiency, is the actual advantage 
over the pseudoblocking mode from Section 1, since both modes 
rely on the identical underlying wait mechanism.&]
[s5; [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()] is a special case within this model, because unlike 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()], it has no natural completion condition of its own; 
its internal end`-of`-frame check is written to always report 
that more input is expected, since a service subscribed to ongoing 
signals is meant to run indefinitely rather than stop after the 
first message. [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()]`'s own boolean return reflects only whether the operation 
was able to start at all, guarding, as described in Section 2, 
against being invoked while a previous operation is still in 
progress; the eventual outcome is a separate question, decided 
from within a callback triggered by whatever event actually signals 
completion; a matching Response signal, for instance. Calling 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Finish`(`)^ F
inish()] from within such a callback records the operation as 
successfully complete and calls [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Abort`(`)^ A
bort()] alongside it, ensuring the connection`'s own pending 
socket read notices the abort state directly and returns promptly 
rather than continuing to wait for further bytes that no longer 
matter. Calling [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Abort`(`)^ A
bort()] on its own, without [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Finish`(`)^ F
inish()], takes a different path: because it leaves status untouched, 
the engine`'s error checking routine, which examines connection 
health only once it finds status still marked as actively working, 
detects the aborted socket on its very next pass and unwinds 
the pump via a [C@5 DBusError] carrying [C@5 ABORTED], the path used 
when the callback determines the operation has failed rather 
than succeeded. Once the pump exits whether driven internally 
by [C@5 Run()] in blocking mode, or externally through repeated 
calls to [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Do`(`)^ D
o()] in non`-blocking mode, [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:IsSuccess`(`)const^ I
sSuccess() ]and [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:IsFailure`(`)const^ I
sFailure()][C  ]report which of the two actually occurred; [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:IsError`(`)const^ I
sError()] remains the broader check, additionally covering failures 
the connection detected entirely on its own, such as a genuine 
socket fault or an exhausted [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Timeout`(int`)^ T
imeout()]. Because [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()] is frequently used to wait for something whose timing 
is outside the application`'s control (a user interacting with 
an external dialog, for instance) it is commonly paired with 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Timeout`(int`)^ T
imeout(Null)] for the duration of the listen specifically, while 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Connect`(const String`&`,bool`)^ C
onnect()] and whatever request precedes it retain their ordinary, 
finite timeout.&]
[s5; For background daemons and graphical interfaces, this asynchronous 
integration is mandatory to keep the window responsive.&]
[s3;:4: 4. Server Dispatch Architecture&]
[s5; When a U`+`+ D`-Bus application acts as an IPC server, it operates 
entirely within the asynchronous execution model described in 
Section 3. During initialization, the application claims a well`-known 
identity on the bus by calling [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:RequestName`(const String`&`)^ R
equestName()] (simply a [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()] issued against the bus daemon) and so subject to 
the same pseudoblocking or asynchronous mechanics as any other 
request, depending on whether [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:NonBlocking`(bool`)^ N
onBlocking()] has been configured. From that point forward, the 
application relies entirely on the primary event loop and the 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Do`(`)^ D
o()] pump to process inbound traffic.&]
[s5; As the engine parses the socket stream, it identifies incoming 
method calls targeting the claimed service name, constructs the 
inbound [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage^ DBus
Message], and fires the [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenMethodCall^ W
henMethodCall] hook, under the same EventLock protection described 
in Section 2. This matters for server implementations specifically: 
if fulfilling an incoming request genuinely requires issuing 
a new, independent synchronous [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()] of your own (proxying the request to another service, 
for instance) that call is subject to exactly the same reentrancy 
restriction a signal handler would face, and needs the same treatment, 
either deferred via [C@5 Ctrl`::PostCallback()] or issued on a separate 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ DBusCo
nnection].&]
[s5; The application`'s local routing logic inspects the message`'s 
interface and member fields, executes the requested operation, 
and constructs a structured payload of return arguments. Because 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:SendReply`(const DBusMessage`&`,const DBusValueArray`&`)^ S
endReply()] and its counterpart for the failure case, [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:SendError`(const DBusMessage`&`,const String`&`,const String`&`)^ S
endError()], both operate through the side`-band mechanism described 
in Section 2 rather than through the primary queue, either can 
be called directly from within the [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenMethodCall^ W
henMethodCall] handler itself with no deferral required. This 
is, in fact, the intended and expected way to use them. [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:SendReply`(const DBusMessage`&`,const DBusValueArray`&`)^ S
endReply()] embeds two pieces of routing information taken from 
the original request: the caller`'s sender identity and the caller`'s 
own outbound serial, placed into the reply`'s destination and 
reply`-serial fields respectively. The bus daemon uses the former 
to deliver the message to the correct connection; the caller`'s 
own pending [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()] pump uses the latter to recognize the reply as the 
one it has been waiting for. [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:SendError`(const DBusMessage`&`,const String`&`,const String`&`)^ S
endError()] follows the identical routing pattern, but constructs 
a DBus [C@5 ERROR]`-type message instead, carrying an error name 
and an optional human`-readable description, for the cases where 
the request cannot be fulfilled as asked.]]
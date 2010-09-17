function threeservos_init(arg)

% Task scheduling and control.
%
% This example extends the simple PID control example (located in
% $DIR/examples/servo) to the case of three PID-tasks running
% concurrently on the same CPU controlling three different servo
% systems. The effect of the scheduling policy on the global control
% performance is demonstrated.

% Initialize TrueTime kernel

ttSetNumberOfCPUs(3)

switch arg
 case 1
  ttInitKernel('prioDM')
  codefcn = 'pid_code1';
 case 2
  ttInitKernel('prioEDF')
  codefcn = 'pid_code1';
 case 3
  ttInitKernel('prioEDF')
  codefcn = 'pid_code1';
 case 4
  ttInitKernel('prioEDF')
  codefcn = 'pid_code1';
 case 5
  ttInitKernel('prioEDF')
  codefcn = 'pid_code2';
 otherwise
  error('Illegal init argument')
end

% Task parameters
starttimes = [0 0 0];
periods = [0.006 0.005 0.004];
tasknames = {'pid_task1', 'pid_task2', 'pid_task3'};

% Create the three tasks
for i = 1:3
  data.K = 0.96;
  data.Ti = 0.12;
  data.Td = 0.049;
  data.beta = 0.5;
  data.N = 10;
  data.h = periods(i);
  data.u = 0;
  data.Iold = 0;
  data.Dold = 0;
  data.yold = 0;
  data.rChan = 1;
  data.yChan = i+1;
  data.uChan = i;
  data.late = 0;
  
  ttCreatePeriodicTask(tasknames{i}, starttimes(i), periods(i), codefcn, data);

  ttSetCPUAffinity(tasknames{i},i)
  
  ttCreateLog(tasknames{i},1,['response' num2str(i)],1000)
end

if arg == 3
  ttCreateHandler('dl_miss_handler', 1, 'dl_miss_code')
  for i = 1:3
    ttAttachDLHandler(tasknames{i}, 'dl_miss_handler')
  end
end

if arg == 4
  ttCreateCBS('cbs', 0.001, 0.004)   % Allow max 25% utilization
  ttAttachCBS(tasknames{3}, 'cbs')   % for pid_task3
end

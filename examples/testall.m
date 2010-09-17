clear all
ccd = pwd;
try
  disp('testing examples/simple/matlab...')
  cd simple/matlab
  open_system('simple')
  sim('simple')
  close_system('simple')
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/simple/c++...')
  cd simple/c++
  open_system('simple')
  sim('simple')
  close_system('simple')
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/servo/matlab...')
  cd servo/matlab
  open_system('servo')
  sim('servo')
  close_system('servo')
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/servo/c++...')
  cd servo/c++
  open_system('servo')
  sim('servo')
  close_system('servo')
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/threeservos/matlab...')
  cd threeservos/matlab
  open_system('threeservos')
  sim('threeservos')
  close_system('threeservos')
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/threeservos/c++...')
  cd threeservos/c++
  open_system('threeservos')
  sim('threeservos')
  close_system('threeservos')
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/networked/matlab...')
  cd networked/matlab
  open_system('networked')
  sim('networked')
  close_system('networked')
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/networked/c++...')
  cd networked/c++
  open_system('networked')
  sim('networked')
  close_system('networked')
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

% clear all
% ccd = pwd;
% try
%   disp('testing examples/networked/standalone...')
%   cd networked/standalone
%   open_system('networked')
%   sim('networked')
%   close_system('networked')
%   disp('OK')
% catch
%   disp('Failed!')
% end
% cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/AODV...')
  cd AODV
  open_system('AODV')
  sim('AODV',5)
  close_system('AODV')
  close all
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/wireless...')
  cd wireless
  open_system('wireless')
  sim('wireless',5)
  close_system('wireless')
  close all
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

clear all
ccd = pwd;
try
  disp('testing examples/soccer...')
  cd soccer
  open_system('soccer')
  sim('soccer',10)
  close_system('soccer')
  close all
  disp('OK')
catch
  disp('Failed!')
end
cd(ccd)

% clear all
% ccd = pwd;
% try
%   disp('testing examples/RUNES_demo...')
%   cd RUNES_demo
%   open_system('RUNES_demo')
%   sim('RUNES_demo',10)
%   close_system('RUNES_demo')
%   close all
%   disp('OK')
% catch
%   disp('Failed!')
% end
% cd(ccd)

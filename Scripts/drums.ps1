Write-Host -NoNewLine 'Check if everything was connected. Then press any key to continue...';
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');

& 'C:\Program Files (x86)\Tobias Erichsen\loopMIDI\loopMIDI.exe'
& 'D:\Jasiek\Workspace\EDrums\hairless-midiserial-0.4-windows\hairless-midiserial\hairless-midiserial.exe'
& 'C:\Program Files\REAPER (x64)\reaper.exe' D:\Nightkeyman\Perkusja\Drums_MIDI.rpp
& osk
Write-Host -NoNewLine 'Check if everything was connected. Then press any key to continue...';
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');

& $env:WORKSPACE\EDrums\Tools\TobiasErichsen\loopMIDI\loopMIDI.exe
& $env:WORKSPACE\EDrums\Tools\hairless-midiserial\hairless-midiserial.exe
& 'C:\Program Files\REAPER (x64)\reaper.exe' D:\Data\Recordings\Others\Drums\Drums_MIDI.rpp
& 'C:\Users\Jan\AppData\Roaming\Spotify\Spotify.exe'
& osk
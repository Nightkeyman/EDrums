Write-Host '>> Check if everything was connected. Then press any key to continue...';
Write-Host '- mouse';
Write-Host '- monitor cabel';
Write-Host '- Drum kit USB';
Write-Host '- Arduino USB';
Write-Host '- Headphones plugged to interface';
$null = $Host.UI.RawUI.ReadKey('NoEcho,IncludeKeyDown');

Write-Host '>> Launching...';
& $env:WORKSPACE\EDrums\Tools\TobiasErichsen\loopMIDI\loopMIDI.exe
& $env:WORKSPACE\EDrums\Tools\hairless-midiserial\hairless-midiserial.exe
& 'C:\Program Files\REAPER (x64)\reaper.exe' D:\Data\Recordings\Others\Drums\Drums_MIDI.rpp
& 'C:\Users\Jan\AppData\Roaming\Spotify\Spotify.exe'
& osk
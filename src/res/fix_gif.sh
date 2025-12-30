#ffmpeg -i $1 -vf "split[s0][s1];[s0]palettegen=stats_mode=full:reserve_transparent=off:transparency_color=ffffff[p];[s1][p]paletteuse" t_$2
#ffmpeg -i t_$2 -filter_complex "color=white[bg];[bg][0]overlay=format=rgb" $2

#ffmpeg -f lavfi -i color=c=white -i $1 -filter_complex "[0][1]scale2ref[bg][gif];[bg]setsar=1[bg];[bg][gif]overlay=shortest=1:format=auto,split[s0][s1];[s0]palettegen[p];[s1][p]paletteuse" $2

gifsicle --colors=255 -o pfix_$2 $1
gifsicle --unoptimize -o pfix_$2 $2

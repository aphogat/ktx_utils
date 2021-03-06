#!/bin/sh
# This script creates all the KTXs necessary to test the ASTC functionality in HW.

#GL_COMPRESSED_RGBA_ASTC_4x4_KHR   0x93B0
#GL_COMPRESSED_RGBA_ASTC_5x4_KHR   0x93B1
#GL_COMPRESSED_RGBA_ASTC_5x5_KHR   0x93B2
#GL_COMPRESSED_RGBA_ASTC_6x5_KHR   0x93B3
#GL_COMPRESSED_RGBA_ASTC_6x6_KHR   0x93B4
#GL_COMPRESSED_RGBA_ASTC_8x5_KHR   0x93B5
#GL_COMPRESSED_RGBA_ASTC_8x6_KHR   0x93B6
#GL_COMPRESSED_RGBA_ASTC_8x8_KHR   0x93B7
#GL_COMPRESSED_RGBA_ASTC_10x5_KHR  0x93B8
#GL_COMPRESSED_RGBA_ASTC_10x6_KHR  0x93B9
#GL_COMPRESSED_RGBA_ASTC_10x8_KHR  0x93BA
#GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0x93D0
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR 0x93D1
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR 0x93D2
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR 0x93D3
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR 0x93D4
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR 0x93D5
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR 0x93D6
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR 0x93D7
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR 0x93D8
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR 0x93D9
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR 0x93DA
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR 0x93DB
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR 0x93DC
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR 0x93DD

ASTC_2D_Enum=(
	"0x93B0"
	"0x93B1"
	"0x93B2"
	"0x93B3"
	"0x93B4"
	"0x93B5"
	"0x93B6"
	"0x93B7"
	"0x93B8"
	"0x93B9"
	"0x93BA"
	"0x93BB"
	"0x93BC"
	"0x93BD"
	"0x93D0"
	"0x93D1"
	"0x93D2"
	"0x93D3"
	"0x93D4"
	"0x93D5"
	"0x93D6"
	"0x93D7"
	"0x93D8"
	"0x93D9"
	"0x93DA"
	"0x93DB"
	"0x93DC"
	"0x93DD"
)

Blk2d=(
	"4x4"
	"5x4"
	"5x5"
	"6x5"
	"6x6"
	"8x5"
	"8x6"
	"8x8"
	"10x5"
	"10x6"
	"10x8"
	"10x10"
	"12x10"
	"12x12"
)

#GL_COMPRESSED_RGBA_ASTC_3x3x3_OES 0x93C0
#GL_COMPRESSED_RGBA_ASTC_4x3x3_OES 0x93C1
#GL_COMPRESSED_RGBA_ASTC_4x4x3_OES 0x93C2
#GL_COMPRESSED_RGBA_ASTC_4x4x4_OES 0x93C3
#GL_COMPRESSED_RGBA_ASTC_5x4x4_OES 0x93C4
#GL_COMPRESSED_RGBA_ASTC_5x5x4_OES 0x93C5
#GL_COMPRESSED_RGBA_ASTC_5x5x5_OES 0x93C6
#GL_COMPRESSED_RGBA_ASTC_6x5x5_OES 0x93C7
#GL_COMPRESSED_RGBA_ASTC_6x6x5_OES 0x93C8
#GL_COMPRESSED_RGBA_ASTC_6x6x6_OES 0x93C9
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_3x3x3_OES 0x93E0
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x3x3_OES 0x93E1
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x3_OES 0x93E2
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x4_OES 0x93E3
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4x4_OES 0x93E4
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x4_OES 0x93E5
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x5_OES 0x93E6
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5x5_OES 0x93E7
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x5_OES 0x93E8
#GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x6_OES 0x93E9

ASTC_3D_Enum=(
	"0x93C0"
	"0x93C1"
	"0x93C2"
	"0x93C3"
	"0x93C4"
	"0x93C5"
	"0x93C6"
	"0x93C7"
	"0x93C8"
	"0x93C9"
	"0x93E0"
	"0x93E1"
	"0x93E2"
	"0x93E3"
	"0x93E4"
	"0x93E5"
	"0x93E6"
	"0x93E7"
	"0x93E8"
	"0x93E9"
)

Blk3d=(
	"3x3x3"
	"4x3x3"
	"4x4x3"
	"4x4x4"
	"5x4x4"
	"5x5x4"
	"5x5x5"
	"6x5x5"
	"6x6x5"
	"6x6x6"
)

Fmt=(
	"hdr"
	"ldrl"
	"ldrs"
)

Dims=(
	"2D"
	"SLICED3D"
	"3D"
)

# Executables
encoder=./astc-encoder/Source/astcenc
ktx_fix=./fix_mali_ktx
ktx_gen=./gen_ktx
mip_gen=./gen_mipmaps
arr_gen=./gen_array

# Global variables
compressed_dir=compressed
decompressed_dir=decompressed
failed=0

# Input-based variables
inFileHead=${1%%.*}
inFileExt=${1##$inFileHead}
img_w=`identify -format "%[fx:w]" $1`
img_h=`identify -format "%[fx:h]" $1`
format=${2}
max_dim=$(( $img_w < $img_h ? $img_h : $img_w ))
MAX_LEVEL=`echo "a=l($max_dim)/l(2); scale=0; a/1" | bc -l`
DEPTH=8

# Function definitions
function run_cmd {
	cmd=$1
	output=$2
	echo $cmd
	eval $cmd
	if [ ! -e "$output" ]; then
		echo $cmd
		echo "File not created: $output"
		failed=1
	fi
}

function minify {
	img_w=$1
	if [ $(($img_w >> 1)) -eq 0 ]; then
		img_w=1
	else
		img_w=$(($img_w >> 1))
	fi
	echo $img_w
}

# Generate mipmapped KTXs for each configuration combination:
# * HDR, LDR-SRGB, LDR-LINEAR
# * Blocks from 4x4 to 12x12 for 2D
# * Blocks from 3x3x3 to 6x6x6 for 3D
# * LODs specific to starting image size
# * Compressed vs decompressed
function create_ktx_for_fmt {
	i=$1
	j=$2
	switch="" # HDR by default
	decopts=""
	encopts="-veryfast"
	astc_fmt_array_offset=0
	blocks=13

	if [ "$j" = "3D" ]; then
		blocks=9
	fi

	if [ "$i" = "ldrs" ]; then
		switch="s"
		if [ "$j" = "2D" -o "$j" = "SLICED3D" ]; then
			astc_fmt_array_offset=14
		elif [ "$j" = "3D" ]; then
			astc_fmt_array_offset=10
		fi
	elif [ "$i" = "ldrl" ]; then
		switch="l"
	else
		decopts+=" -hdr"
		encopts+=" -hdr"
	fi

	# Generate all the block configurations.
	for n in $(seq 0 $blocks); do
		depth=$DEPTH
		if [ "$j" = "2D" -o "$j" = "SLICED3D" ]; then
		        blk=${Blk2d[$n]}
		else
		        blk=${Blk3d[$n]}
		fi

		outFileHead=${inFileHead}-$blk
		outDirC=$compressed_dir/$j/$i/$blk
		outDirD=$decompressed_dir/$j/$i/$blk
		mkdir -p $outDirD $outDirC

		# Generate all the Level of Details
		for lod in $(seq 0 $MAX_LEVEL); do
			# Debug why do we get segfault with depth=1
			if [ "$depth" = "1" ]; then
				break;
			fi

			if [ "$j" = "3D" -o "$j" = "SLICED3D" ]; then
				encopts="-array $depth -veryfast"
				if [ "$i" = "hdr" ]; then
					encopts+=" -hdr"
				fi
				depth=$((depth/2))
			fi

			# Choose the right base file
			inFile=${inFileHead}-${lod}$inFileExt

			# Setup output files
			outFile=${outFileHead}-$lod
			outFileA=$outDirC/${outFile}.astc
			outFileCK=$outDirC/${outFile}.ktx
			outFileK=$outDirD/${outFile}.ktx

			# Create an ASTC from the input file.
			run_cmd "$encoder -c$switch $inFile $outFileA $blk $encopts > /dev/null" $outFileA
			if [ $failed -eq 1 ]; then
				break
			fi

			# Create a KTX from the ASTC.
			run_cmd "$encoder -d$switch $outFileA $outFileK $decopts > /dev/null" $outFileK
			if [ $failed -eq 1 ]; then
				break
			fi

			if [ "$j" = "2D" -o "$j" = "SLICED3D" ]; then
				# Wrap the compressed ASTC in a KTX.
				run_cmd "$ktx_gen $outFileA $outFileCK ${ASTC_2D_Enum[($n + $astc_fmt_array_offset)]} > /dev/null" $outFileCK
			else
				# Wrap the compressed ASTC in a KTX.
				run_cmd "$ktx_gen $outFileA $outFileCK ${ASTC_3D_Enum[($n + $astc_fmt_array_offset)]} > /dev/null" $outFileCK
			fi

			if [ $failed -eq 1 ]; then
				break
			fi

			# Delete the ASTC.
			rm $outFileA
		done

		# Generate a mipmap from all the miplevel KTXs.
		if [ $failed -eq 0 ]; then
		run_cmd "$mip_gen $outDirC/ ../${outFileHead}.ktx" $outDirC/../${outFileHead}.ktx
		if [ $failed -eq 1 ]; then
				rm $outDirC/${outFileHead}.ktx
		fi

		run_cmd "$mip_gen $outDirD/ ../${outFileHead}.ktx" $outDirD/../${outFileHead}.ktx
		if [ $failed -eq 1 ]; then
				rm $outDirD/${outFileHead}.ktx
		fi

			# Mali-decompressed sRGB & HDR ASTCs have the wrong glInternalFormat; correct this.
			if [ "$i" = "hdr" -o "$i" = "ldrs" ]; then
				$ktx_fix $outDirD/../${outFileHead}.ktx > /dev/null
				#echo "$ktx_fix $outDirD/../${outFileHead}.ktx"
			fi

			# Delete the miplevels KTXs.
                                rm -rf $outDirC $outDirD
		fi
	done

	if [ "$j" = "2D" ]; then
		# Generate the 2D texture array
		run_cmd "$arr_gen $decompressed_dir/$j/$i/${inFileHead}.ktx $MAX_LEVEL `ls -vd $decompressed_dir/$j/$i/* | grep ktx`" $decompressed_dir/$j/$i/${inFileHead}.ktx
		if [ $failed -eq 1 ]; then
		rm $outDirD/${inFileHead}.ktx
		fi
	fi
}


# MAIN
# Generate all miplevels from the source image
for lod in $(seq 0 $MAX_LEVEL); do

	# Resize the image for current level of detail (LOD).
	# Change the hue of each miplevel
	lod_out=${inFileHead}-${lod}$inFileExt
	percent=$( echo "scale = 4; a = $lod*200/$MAX_LEVEL + 100; if (a < 200) a else a - 200" | bc )
	convert -modulate 100,100,$percent ${inFileHead}$inFileExt"[${img_w}x${img_h}!]" $lod_out
	echo "LOD-$lod dimensions are: ${img_w}x${img_h}"
	for depth in $(seq 0 $DEPTH); do
		cp $lod_out ${inFileHead}-${lod}_$depth$inFileExt
	done
	# Generate next LOD dimensions.
	img_w=$(minify $img_w)
	img_h=$(minify $img_h)
done


# Generate a specific format or all formats
if [ -n "$format" ] && [ -n "$dims" ]; then
	create_ktx_for_fmt $format $dims
else
	for j in ${Dims[@]}; do
		for i in ${Fmt[@]}; do
			create_ktx_for_fmt $i $j
		done
	done
fi

# Delete each level of detail
for lod in $(seq 0 $MAX_LEVEL); do
	rm ${inFileHead}-${lod}$inFileExt
done

for lod in $(seq 0 $MAX_LEVEL); do
	for depth in $(seq 0 $DEPTH); do
		rm ${inFileHead}-${lod}_$depth$inFileExt
	done
done

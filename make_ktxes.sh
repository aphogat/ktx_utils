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
#GL_COMPRESSED_RGBA_ASTC_3x3x3_OES 0x93C0
#GL_COMPRESSED_RGBA_ASTC_4x3x3_OES 0x93C1
#GL_COMPRESSED_RGBA_ASTC_4x4x3_OES 0x93C2
#GL_COMPRESSED_RGBA_ASTC_4x4x4_OES 0x93C3
#GL_COMPRESSED_RGBA_ASTC_5x4x4_OES 0x93C4
#GL_COMPRESSED_RGBA_ASTC_5x5x4_OES 0x93C5
#GL_COMPRESSED_RGBA_ASTC_5x5x5_OES 0x93C6
#GL_COMPRESSED_RGBA_ASTC_6x5x5_OES 0x93C7
#GL_COMPRESSED_RGBA_ASTC_6x6x5_OES 0x93C8
#GL_COMPRESSED_RGBA_ASTC_7x6x6_OES 0x93C9
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

Fmt=(
	"hdr"
	"ldrl"
	"ldrs"
)

Dims=(
	"2D"
	"3D"
)

Blk=(
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

# Executables
encoder=./astcenc
ktx_fix=./fix_mali_ktx
ktx_gen=./gen_ktx
mip_gen=./gen_mipmaps

# Global variables
MAX_LEVEL=100
compressed_dir=../compressed
decompressed_dir=../decompressed

# Input-based variables
inFileHead=${1%%.*}
img_w=`identify -format "%[fx:w]" $1`
img_h=`identify -format "%[fx:h]" $1`

# Generate all miplevels from the source image
for lod in $(seq 0 $MAX_LEVEL); do

	# Resize the image for current level of detail (LOD).
	convert ${inFileHead}.jpg"[${img_w}x${img_h}!]" ${inFileHead}-${lod}.jpg

	# Stop generating miplevels when reaching dimensions 1x1.
	echo "LOD-$lod dimensions are: ${img_w}x${img_h}"
	if [ $img_w -eq 1 -a $img_h -eq 1 ]; then
		MAX_LEVEL=$lod
		break
	fi

	# Generate next LOD dimensions.
	if [ $(($img_w >> 1)) -eq 0 ]; then
		img_w=1
	else
		img_w=$(($img_w >> 1))
	fi
		
	if [ $(($img_h >> 1)) -eq 0 ]; then
		img_h=1
	else
		img_h=$(($img_h >> 1))
	fi

done


# Generate mipmapped KTXs for each configuration combination:
# * HDR, LDR-SRGB, LDR-LINEAR
# * Blocks from 4x4 to 12x12
# * LODs specific to starting image size
# * Compressed vs decompressed
for i in ${Fmt[@]}; do
	switch="" # HDR by default
	decopts="-veryfast"
	encopts="-veryfast"
	astc_fmt_array_offset=0
	if [ "$i" = "ldrs" ]; then
		switch="s"
		astc_fmt_array_offset=14
		#encopts+=" -srgb"
		#decopts+=" -srgb"
	elif [ "$i" = "ldrl" ]; then
		switch="l"
	else
		decopts+=" -hdr"
		encopts+=" -hdr"
	fi

	# Generate all the block configurations.
	for n in $(seq 0 13); do
		failed=0
		blk=${Blk[$n]}
		outFileHead=${inFileHead}-$blk
		outDirC=$compressed_dir/$i/$blk
		outDirD=$decompressed_dir/$i/$blk
		mkdir -p $outDirD $outDirC

		# Generate all the Level of Details
		for lod in $(seq 0 $MAX_LEVEL); do

			# Choose the right base file
			inFile=${inFileHead}-${lod}.jpg

			# Setup output files
			outFile=${outFileHead}-$lod
			outFileA=$outDirC/${outFile}.astc
			outFileCK=$outDirC/${outFile}.ktx
			outFileK=$outDirD/${outFile}.ktx

			# Create an ASTC from the input file.
			$encoder -c$switch $inFile $outFileA $blk $encopts > /dev/null
			if [ ! -e "$outFileA" ]; then
				echo "ASTC file not created! $outFileA"
				echo "$encoder -c$switch $inFile $outFileA $blk $encopts > /dev/null"
				failed=1
				break
			fi

			# Create a KTX from the ASTC.
			$encoder -d$switch $outFileA $outFileK $decopts > /dev/null
			if [ ! -e "$outFileK" ]; then
				echo "KTX (RGB) file not created! $outFileA"
				echo "$encoder -d$switch $outFileA $outFileK $decopts > /dev/null"
				failed=1
				break
			fi

			# Mali-decompressed HDR ASTCs have the wrong glInternalFormat; correct this.
			if [ "$i" = "hdr" ]; then
				$ktx_fix $outFileK > /dev/null
			fi

			# Wrap the compressed ASTC in a KTX.
			$ktx_gen $outFileA $outFileCK ${ASTC_2D_Enum[($n + $astc_fmt_array_offset)]} > /dev/null
			if [ ! -e "$outFileCK" ]; then
				echo "KTX (ASTC) file not created! $outFileA"
				echo "$ktx_gen $outFileA $outFileCK ${ASTC_2D_Enum[($n + $astc_fmt_array_offset)]} > /dev/null"
				failed=1
				break
			fi

			# Delete the ASTC.
			rm $outFileA
		done

		# Generate a mipmap from all the miplevel KTXs.
		if [ $failed -eq 0 ]; then
			$mip_gen $outDirC/ ../${outFileHead}.ktx
			$mip_gen $outDirD/ ../${outFileHead}.ktx
			if [ ! -e "$outDirC/../${outFileHead}.ktx" ]; then
				echo "$mip_gen $outDirC/ ${inFileHead}.ktx"
			fi

			# Delete the miplevels KTXs.
			rm -rf $outDirC $outDirD
		fi
	done
		
done

# Delete each level of detail
for lod in $(seq 0 $MAX_LEVEL); do
	rm ${inFileHead}-${lod}.jpg
done

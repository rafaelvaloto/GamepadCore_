// Copyright (c) 2025 Rafael Valoto. All Rights Reserved.
// Project: GamepadCore
// Description: Cross-platform library for DualSense and generic gamepad input support.
// Targets: Windows, Linux, macOS.
#pragma once
#include <cstdint>

namespace GCoreUtils
{

	class CR32
	{
	public:
		/**
		 * @fn CR32::Compute
		 *
		 * Computes the CRC32 hash value of the specified buffer based on the provided length.
		 * Utilizes a precomputed hash lookup table (`HashTable`) and an initial seed value (`CRCSeed`)
		 * to calculate the checksum of the input data.
		 *
		 * @param Buffer A pointer to the input buffer containing the data to be hashed.
		 * @param Len The length of the buffer to be processed.
		 * @return The computed CRC32 hash as a 32-bit unsigned integer.
		 *
		 * This function performs byte-by-byte operations using XOR computations and the preinitialized
		 * lookup table. It ensures an efficient and consistent hash generation for the given input.
		 */
		static std::uint32_t Compute(const unsigned char* Buffer, std::size_t Len);

	private:
		/**
		 * @var FPlayStationOutputComposer::CRCSeed
		 *
		 * The predefined seed value used for CRC32 hash computation.
		 * Acts as the initial value for the hash generation algorithm employed in the
		 * Compute method. Ensures consistent and reliable hash results by providing a
		 * stable starting point.
		 */
		static constexpr std::uint32_t CRCSeed = 0xeada2d49;
		/**
		 * @variable HashTable
		 *
		 * Represents a precomputed hash lookup table specifically used within
		 * `FGamepadOutput` for optimizing hash calculations. This table
		 * is a key component in facilitating fast cryptographic or hash operations.
		 *
		 * The `HashTable` variable contains a fixed array of 256 32-bit values,
		 * each defined as hexadecimal constants. These values are statically
		 * initialized, ensuring deterministic results for hash-related operations. It
		 * is typically used in operations related to device communication, such as
		 * data integrity checks or packet processing within DualSense HID
		 * interactions.
		 *
		 * The structure of the table ensures constant-time retrieval of hash values,
		 * making it integral to performance-critical systems where such operations
		 * are frequent.
		 */
		static const std::uint32_t HashTable[256];
	};

} // namespace GCoreUtils

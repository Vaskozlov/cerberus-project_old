#ifndef CERBERUS_RANDOM_VALUES_HPP
#define CERBERUS_RANDOM_VALUES_HPP

#include <cerberus/cerberus.hpp>

namespace cerb::debug
{
    constexpr std::array<u8, 128> RandomBytes = {
        0xb4U, 0xd8U, 0x1dU, 0x45U, 0xdcU, 0xd9U, 0x7eU, 0xd0U, 0x94U, 0x42U, 0xe9U, 0x8cU, 0xe3U,
        0x69U, 0x4dU, 0x11U, 0x88U, 0x72U, 0xa9U, 0x78U, 0x4dU, 0xb2U, 0x6bU, 0x4dU, 0xb4U, 0x4aU,
        0xa2U, 0x96U, 0x13U, 0x34U, 0x94U, 0x4aU, 0x79U, 0xccU, 0xb9U, 0xe7U, 0xb3U, 0x1fU, 0xadU,
        0xcbU, 0x33U, 0x2fU, 0x37U, 0x10U, 0xccU, 0x6aU, 0x8dU, 0x24U, 0x75U, 0x60U, 0x64U, 0x9fU,
        0x20U, 0x27U, 0x1dU, 0x22U, 0xfaU, 0xfcU, 0x8dU, 0xd7U, 0x70U, 0x65U, 0x8cU, 0x9aU, 0xeU,
        0xddU, 0x41U, 0xffU, 0xdU,  0xd6U, 0xe2U, 0xafU, 0xbU,  0xefU, 0xa6U, 0x77U, 0xbfU, 0x6aU,
        0x94U, 0x6bU, 0x8U,  0xa3U, 0xc8U, 0xadU, 0x67U, 0x7aU, 0x1cU, 0xe3U, 0xe4U, 0x6dU, 0xe3U,
        0x11U, 0x13U, 0x51U, 0x31U, 0x65U, 0xeaU, 0xaaU, 0xbaU, 0xe2U, 0xedU, 0x2dU, 0x17U, 0x2fU,
        0x2bU, 0xc1U, 0x3dU, 0x9eU, 0xe1U, 0x2U,  0x76U, 0x5aU, 0x13U, 0x7aU, 0x23U, 0xb5U, 0x25U,
        0xf3U, 0xaU,  0x1bU, 0x54U, 0x27U, 0xb4U, 0xb4U, 0x71U, 0x42U, 0x8eU, 0x8bU
    };

    constexpr std::array<u16, 128> RandomWords = {
        0xb055U, 0xf69eU, 0xf74dU, 0xf17cU, 0x8824U, 0xcd7bU, 0xc64eU, 0x39b2U, 0x932eU, 0x6a96U,
        0x173cU, 0x121dU, 0x8a7eU, 0x5592U, 0x4fefU, 0xdd9cU, 0xaa49U, 0x5fabU, 0xf3e0U, 0x9094U,
        0xd056U, 0x8534U, 0xca11U, 0x9a23U, 0x2232U, 0x3074U, 0xc6a4U, 0xd6e3U, 0x4c8eU, 0x801aU,
        0x1deaU, 0xf797U, 0xc9f8U, 0x9e62U, 0x574dU, 0x6bf7U, 0x62f0U, 0xc4eeU, 0x9a22U, 0xf8f0U,
        0xda4dU, 0xf97eU, 0x20dU,  0x1625U, 0xc621U, 0x3c37U, 0x8c32U, 0x6435U, 0x21bdU, 0xdcc4U,
        0xd191U, 0xcb5bU, 0xddbcU, 0x8d53U, 0xf46U,  0xc4feU, 0xa612U, 0x7820U, 0x4b3bU, 0x4bd5U,
        0xdf86U, 0xb5f3U, 0xe77cU, 0x96ceU, 0xc8ecU, 0x6b52U, 0xb52U,  0x6c53U, 0xc808U, 0xcbbcU,
        0x6680U, 0x42b5U, 0x1d6eU, 0x690cU, 0xd90eU, 0x24e9U, 0x8e0eU, 0xb7ecU, 0x8eb7U, 0x7babU,
        0x9d75U, 0x694fU, 0xaeefU, 0x233cU, 0x5a62U, 0x631U,  0x8ebcU, 0xc772U, 0xa6a6U, 0x5069U,
        0x6262U, 0x5f35U, 0x9271U, 0xcc01U, 0x9a48U, 0xa416U, 0x73b5U, 0x8686U, 0x9b2fU, 0xf786U,
        0x8cbU,  0xc6f2U, 0x3ea5U, 0xa15eU, 0x94b4U, 0x87d5U, 0xa82U,  0xc98dU, 0x9ff3U, 0xba8fU,
        0x6e4fU, 0xc8c5U, 0x2fa0U, 0x6b47U, 0x1f24U, 0x71ccU, 0x663dU, 0x33e4U, 0x4aaeU, 0x1995U,
        0xe709U, 0x5bc3U, 0x8234U, 0x965dU, 0xad2eU, 0x8944U, 0xf93aU, 0x7253U
    };

    constexpr std::array<u32, 128> RandomDWords = {
        0x4b9881fU,  0x2ae4e3d5U, 0xbea11781U, 0xeaf4ac68U, 0x3cbc5584U, 0x19fbc412U, 0xbbf38df0U,
        0xb9fe8c4bU, 0x1200bbb0U, 0x97a7b89U,  0x320e69a9U, 0xcbfcdc91U, 0x97b1cff5U, 0xcaea6a0fU,
        0xcef6e090U, 0xb32c6413U, 0x4405870U,  0x49e714e5U, 0x28f4997dU, 0x4afe32e7U, 0x4563966cU,
        0xf5e35e06U, 0xae45bc16U, 0x2eff1848U, 0x71075207U, 0x6938d0f0U, 0x814d8093U, 0xeb4d6c58U,
        0xe14cc70U,  0xe159358fU, 0x61c34299U, 0xcdf86299U, 0xaacd0f75U, 0xccaac750U, 0x6e35d0ecU,
        0x89d712b7U, 0x41d6eb8eU, 0x7415ea53U, 0x4481585bU, 0x82007b65U, 0x432bd31fU, 0xdc5db4b8U,
        0x53a3d836U, 0x9130541dU, 0xc63911e5U, 0xe10ec2a3U, 0xd3076a17U, 0x4a3f871aU, 0xe626679fU,
        0x4d210c06U, 0xaa934daaU, 0xfeac79feU, 0x5c0bad87U, 0xf3e4bf91U, 0x816bff34U, 0x84cd9638U,
        0x740b9fa3U, 0xed1de602U, 0x6b18e481U, 0x67835567U, 0x44940536U, 0x4110e23fU, 0xe05f51e3U,
        0xe88a4357U, 0x7ec21c32U, 0x391ef621U, 0xaec77b1eU, 0xee76f880U, 0x331902daU, 0xc9b192b2U,
        0xad576d3aU, 0x48e9dc21U, 0xfb5a6ff0U, 0x55af183dU, 0x7bce6099U, 0xf534b20bU, 0x8f1ab46fU,
        0x235970e0U, 0x972d4b1dU, 0x6930b51U,  0x74fb5d2fU, 0xaf887137U, 0x6e31886bU, 0xf58488f6U,
        0xb456648U,  0x1bdb3fe1U, 0xe4687681U, 0x7fdfe043U, 0x6c5c2072U, 0xfbffe61cU, 0xfdb53abfU,
        0x8d7c55dfU, 0xf81fd7ddU, 0x86172fa6U, 0x9a7b4127U, 0x6bc20ee1U, 0xdbd46375U, 0x5ee26670U,
        0xebf1575aU, 0x6143eb6dU, 0x6cf4773U,  0xc3900a3dU, 0x460b5613U, 0xbc0242dbU, 0x24595221U,
        0xb4c66e1eU, 0xefa33184U, 0x8616b071U, 0x45dcac83U, 0xb10aae0aU, 0x97dd4f73U, 0x1f1569b3U,
        0xf9b177cfU, 0xbcd4e012U, 0xc4a18ef6U, 0xd875305aU, 0x38c0558dU, 0x561387b6U, 0xd211bf05U,
        0x91291485U, 0xa1039707U, 0x74942015U, 0xca4d4d6aU, 0x35f78170U, 0x9cb6eb0aU, 0xe4ece225U,
        0x20204b1fU, 0x661d55daU
    };

#if CERBLIB_64BIT
    constexpr std::array<u64, 128> RandomQWords = {
        0x28a17fd8defb7d1U,  0xcb7c7abc95adb2c0U, 0x693f988b0d39b161U, 0xe1ab038c70e011b6U,
        0x7bd0c0ad88c882fdU, 0x2cf39ee604e431d1U, 0x8f886b0a06b6feebU, 0x81926411254b502dU,
        0xba9853ce1adaa4ecU, 0xa10b8d333fda86ecU, 0x1e797138e4c87e45U, 0x2f4e7b49ec3cb2c5U,
        0x700f571bb48aa4a7U, 0x65ea89aa6e47f099U, 0x3bab8abf43226414U, 0xf5d7c8dd2c8d305bU,
        0x62c0f0a0442991b7U, 0xb55952dca4628141U, 0x8983f5fdf8d9101aU, 0xff15d426f2687649U,
        0xb5225a86a6b91a76U, 0xac4d1919e4b61bf6U, 0xb507126f27608677U, 0x26d8ca4b31852b67U,
        0x53da47b9ae73b36cU, 0x5b45c0db1989ce4fU, 0x82f8b46c95f77304U, 0x16202fa0a2ca6a75U,
        0x6a4b79aee138331U,  0x42d86488f2a067fbU, 0x67df6791c572fd24U, 0x7f9583df077a7518U,
        0x8b4d12f96bc9be74U, 0xc3903f51cf336622U, 0x79bd6334090d91e3U, 0x677d0872ebf2f31aU,
        0x51b14af63039b007U, 0x75211f459f252e5cU, 0xd5d127cf183c6a59U, 0x9d4a93763c391e28U,
        0x30c061a2c4768e9aU, 0xe01f1eb383c1b58bU, 0x11b10106cf01469fU, 0x3e7cc8ad8a586df2U,
        0xa52ce5906d22b4f2U, 0x7203ae2f799b5fffU, 0xb23f6d4b1e826843U, 0x1070d78c3f806ad0U,
        0xcc8f34c20f13ee60U, 0x3ae5ee9eda50cd04U, 0xaf4b3ab24468f019U, 0x4b51f6c5a20b36b2U,
        0x65aac303f20c41ccU, 0xef8d09c1c270b167U, 0xfffcd815a6dc079cU, 0xf5d4fde0ee7bf171U,
        0xcc5f3fc60f20b237U, 0x544ba0d8efa610d2U, 0x933e1b922035cde3U, 0x7f2bb651948f885fU,
        0xe56efe7b6bdef6aaU, 0xb4529150bc51cf31U, 0x634f6e7dee280f67U, 0x7f74a7e119881fdbU,
        0x1783d4edbecf50edU, 0x52acc33057249652U, 0x1237ca1248eedc32U, 0x54e7ff29692613e8U,
        0x2f25a5aa3a2a66abU, 0x93e90ddddcf1a194U, 0x4e4f71c2ba920697U, 0x3572944b689f389bU,
        0x921b8c751a158edU,  0xaf884ba1780a621aU, 0x61181239d283577bU, 0xc34dd95238614a6aU,
        0xf14209aae34b361eU, 0x8a386f1a6d76ad0bU, 0xdaf2162dd02e01fU,  0x61a9e88630666961U,
        0xde28a37270972ba3U, 0xae18d10688ef574eU, 0x4d0ab9d0729e45ecU, 0xaff06245ec1edb6eU,
        0xcb95dd2d1e584aa9U, 0x69bd36123846b1d0U, 0xdf8e21c7e670b04cU, 0x3f2aa5a591872811U,
        0xde8e8da0773c8b2fU, 0x6dfe72b25c7c6b10U, 0xdfeec1e002b1c032U, 0xe34e621c4cca7296U,
        0x5b393f217105674dU, 0xbe3665c5ed67b65bU, 0x321f752e3b53f6b7U, 0xb38fd66a80340297U,
        0x82ee9cfd62bd47daU, 0x45d35f1246ba575U,  0x145a8d6e1aa86fd0U, 0xa12d594f35b0f8bU,
        0xec33986bec8e8296U, 0xbd7b0e399c668036U, 0x5d1e81ef5c3083b6U, 0x6cf688f1cff14ac9U,
        0x7b757018c8fc0ca1U, 0x6da0cf8a8d626d81U, 0xf7413e39e258dffU,  0x39b3a53d64398fbfU,
        0x3acb65db480153ccU, 0x785ba6bbf6531780U, 0x60a06b9a4ba2e7daU, 0xfdf84ed3c1a6dca8U,
        0x4862c9f118469956U, 0xf204e6d466b1442aU, 0xf7b1519252f5fb0eU, 0x95af0ea9cb809e5bU,
        0x7e1a768c9c3890b0U, 0x8c620fe10c807e99U, 0x69b7be521775f8c1U, 0xb97432540e5f478aU,
        0x80470ac41ec1d427U, 0xb6654a0d8a2c8d95U, 0xaf476737f1f55253U, 0x288f2f4c3b4c2da1U,
        0x74f2d28970103d2cU, 0x8ab496deb1d3eb0aU, 0x5529f7204052d521U, 0x79384da40bfe13c9U
    };
#endif
}// namespace cerb::debug

#endif /* CERBERUS_RANDOM_VALUES_HPP */
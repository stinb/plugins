typedef unsigned long u64;

struct hv_vpset {
  u64 format;
  u64 valid_bank_mask;
  u64 bank_contents[];
} __attribute__((packed));

struct hv_tlb_flush_ex {
  u64 address_space;
  u64 flags;
  struct hv_vpset hv_vp_set; // UndCC_Violation
  u64 gva_list[];
} __attribute__((packed));

/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2013  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRUB_UTIL_INSTALL_HEADER
#define GRUB_UTIL_INSTALL_HEADER	1

#include <sys/types.h>
#include <stdio.h>

#include <grub/device.h>
#include <grub/disk.h>
#include <grub/emu/hostfile.h>

#define GRUB_INSTALL_OPTIONS					  \
  { "modules",      GRUB_INSTALL_OPTIONS_MODULES, N_("MODULES"),	  \
    0, N_("pre-load specified modules MODULES"), 1 },			  \
  { "install-modules", GRUB_INSTALL_OPTIONS_INSTALL_MODULES,	  \
    N_("MODULES"), 0,							  \
    N_("install only MODULES and their dependencies [default=all]"), 1 }, \
  { "themes", GRUB_INSTALL_OPTIONS_INSTALL_THEMES, N_("THEMES"),   \
    0, N_("install THEMES [default=%s]"), 1 },	 		          \
  { "fonts", GRUB_INSTALL_OPTIONS_INSTALL_FONTS, N_("FONTS"),	  \
    0, N_("install FONTS [default=%s]"), 1  },	  		          \
  { "locales", GRUB_INSTALL_OPTIONS_INSTALL_LOCALES, N_("LOCALES"),\
    0, N_("install only LOCALES [default=all]"), 1 },			  \
  { "compress", GRUB_INSTALL_OPTIONS_INSTALL_COMPRESS,		  \
    "no,xz,gz,lzo", OPTION_ARG_OPTIONAL,				  \
    N_("compress GRUB files [optional]"), 1 },			          \
    /* TRANSLATORS: platform here isn't identifier. It can be translated. */ \
  { "directory", 'd', N_("DIR"), 0,					\
    N_("use images and modules under DIR [default=%s/<platform>]"), 1 },  \
  { "override-directory", GRUB_INSTALL_OPTIONS_DIRECTORY2,		\
      N_("DIR"), OPTION_HIDDEN,						\
    N_("use images and modules under DIR [default=%s/<platform>]"), 1 },  \
  { "grub-mkimage", GRUB_INSTALL_OPTIONS_GRUB_MKIMAGE,		\
      "FILE", OPTION_HIDDEN, 0, 1 },					\
    /* TRANSLATORS: "embed" is a verb (command description).  "*/	\
  { "pubkey",   'k', N_("FILE"), 0,					\
      N_("embed FILE as public key for signature checking"), 0}

int
grub_install_parse (int key, char *arg);

void
grub_install_push_module (const char *val);

void
grub_install_pop_module (void);

char *
grub_install_help_filter (int key, const char *text,
			  void *input __attribute__ ((unused)));

enum grub_install_plat
  {
    GRUB_INSTALL_PLATFORM_I386_PC,
    GRUB_INSTALL_PLATFORM_I386_EFI,
    GRUB_INSTALL_PLATFORM_I386_QEMU,
    GRUB_INSTALL_PLATFORM_I386_COREBOOT,
    GRUB_INSTALL_PLATFORM_I386_MULTIBOOT,
    GRUB_INSTALL_PLATFORM_I386_IEEE1275,
    GRUB_INSTALL_PLATFORM_X86_64_EFI,
    GRUB_INSTALL_PLATFORM_MIPSEL_LOONGSON,
    GRUB_INSTALL_PLATFORM_SPARC64_IEEE1275,
    GRUB_INSTALL_PLATFORM_POWERPC_IEEE1275,
    GRUB_INSTALL_PLATFORM_MIPSEL_ARC,
    GRUB_INSTALL_PLATFORM_MIPS_ARC,
    GRUB_INSTALL_PLATFORM_IA64_EFI,
    GRUB_INSTALL_PLATFORM_ARM_UBOOT,
    GRUB_INSTALL_PLATFORM_ARM_EFI,
    GRUB_INSTALL_PLATFORM_MIPSEL_QEMU_MIPS,
    GRUB_INSTALL_PLATFORM_MIPS_QEMU_MIPS,
    GRUB_INSTALL_PLATFORM_MAX
  };

enum grub_install_options {
  GRUB_INSTALL_OPTIONS_DIRECTORY = 'd',
  GRUB_INSTALL_OPTIONS_MODULES = 0x201,
  GRUB_INSTALL_OPTIONS_INSTALL_MODULES,
  GRUB_INSTALL_OPTIONS_INSTALL_THEMES,
  GRUB_INSTALL_OPTIONS_INSTALL_FONTS,
  GRUB_INSTALL_OPTIONS_INSTALL_LOCALES,
  GRUB_INSTALL_OPTIONS_INSTALL_COMPRESS,
  GRUB_INSTALL_OPTIONS_DIRECTORY2,
  GRUB_INSTALL_OPTIONS_GRUB_MKIMAGE
};

void
grub_install_args_finish (void);

extern char *grub_install_source_directory;

enum grub_install_plat
grub_install_get_target (const char *src);
void
grub_install_mkdir_p (const char *dst);

void
grub_install_copy_files (const char *src,
			 const char *dst,
			 enum grub_install_plat platid);
char *
grub_install_get_platform_name (enum grub_install_plat platid);

const char *
grub_install_get_platform_cpu (enum grub_install_plat platid);

const char *
grub_install_get_platform_platform (enum grub_install_plat platid);


typedef enum {
  GRUB_COMPRESSION_AUTO,
  GRUB_COMPRESSION_NONE,
  GRUB_COMPRESSION_XZ,
  GRUB_COMPRESSION_LZMA
} grub_compression_t;

void
grub_install_make_image_wrap (const char *dir, const char *prefix,
			      const char *outname, char *memdisk_path,
			      char *config_path,
			      const char *format, int note,
			      grub_compression_t comp);
void
grub_install_make_image_wrap_file (const char *dir, const char *prefix,
				   FILE *fp, const char *outname,
				   char *memdisk_path,
				   char *config_path,
				   const char *mkimage_target, int note,
				   grub_compression_t comp);

int
grub_install_copy_file (const char *src,
			const char *dst,
			int is_critical);

struct grub_install_image_target_desc;

void
grub_install_generate_image (const char *dir, const char *prefix,
			     FILE *out,
			     const char *outname, char *mods[],
			     char *memdisk_path, char **pubkey_paths,
			     size_t npubkeys,
			     char *config_path,
			     const struct grub_install_image_target_desc *image_target,
			     int note,
			     grub_compression_t comp);

const struct grub_install_image_target_desc *
grub_install_get_image_target (const char *arg);

void
grub_util_bios_setup (const char *dir,
		      const char *boot_file, const char *core_file,
		      const char *dest, int force,
		      int fs_probe, int allow_floppy);
void
grub_util_sparc_setup (const char *dir,
		       const char *boot_file, const char *core_file,
		       const char *dest, int force,
		       int fs_probe, int allow_floppy);

char *
grub_install_get_image_targets_string (void);

const char *
grub_util_get_target_dirname (const struct grub_install_image_target_desc *t);

void
grub_install_create_envblk_file (const char *name);

const char *
grub_install_get_default_x86_platform (void);

void
grub_install_register_efi (const char *efidir_disk, int efidir_part,
			   const char *efifile_path,
			   const char *efi_distributor);

void
grub_install_register_ieee1275 (int is_prep, const char *install_device,
				int partno, const char *relpath);

void
grub_install_sgi_setup (const char *install_device,
			const char *imgfile, const char *destname);

int 
grub_install_compress_gzip (const char *src, const char *dest);
int 
grub_install_compress_lzop (const char *src, const char *dest);
int 
grub_install_compress_xz (const char *src, const char *dest);

void
grub_install_get_blocklist (grub_device_t root_dev,
			    const char *core_path, const char *core_img,
			    size_t core_size,
			    void (*callback) (grub_disk_addr_t sector,
					      unsigned offset,
					      unsigned length,
					      void *data),
			    void *hook_data);

void
grub_util_create_envblk_file (const char *name);

void
grub_util_glue_efi (const char *file32, const char *file64, const char *out);

void
grub_util_render_label (const char *label_font,
			const char *label_bgcolor,
			const char *label_color,
			const char *label_string,
			const char *label);

const char *
grub_util_get_target_name (const struct grub_install_image_target_desc *t);

extern char *grub_install_copy_buffer;
#define GRUB_INSTALL_COPY_BUFFER_SIZE 1048576

#endif

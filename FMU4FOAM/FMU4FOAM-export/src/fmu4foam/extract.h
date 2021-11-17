#include <archive.h>
#include <archive_entry.h>
namespace fs = std::filesystem;

static int copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
    la_int64_t offset;

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
        return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
        return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK) {
        fprintf(stderr, "%s\n", archive_error_string(aw));
        return (r);
        }
    }
}

static void extract(const char *filename)
{
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;

    /* Select which attributes we want to restore. */
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_compression_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    if ((r = archive_read_open_filename(a, filename, 10240)))
        exit(1);
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
        break;
        if (r < ARCHIVE_OK)
        fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
        exit(1);
        const char* currentFile = archive_entry_pathname(entry);
        auto fullOutputPath = fs::path("testing") / fs::path(currentFile);
        archive_entry_set_pathname(entry, fullOutputPath.c_str());
        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK)
        fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0) {
        r = copy_data(a, ext);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
            exit(1);
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK)
        fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
        exit(1);
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    exit(0);
}

// static void create_file()
// {
//     struct archive *a;
//     struct archive_entry *entry;

//     a = archive_write_disk_new();
//     archive_write_disk_set_options(a, ARCHIVE_EXTRACT_TIME);

//     entry = archive_entry_new();
//     archive_entry_set_pathname(entry, "my_file.txt");
//     archive_entry_set_filetype(entry, AE_IFREG); // Note 1
//     archive_entry_set_size(ae, 5);  // Note 2
//     archive_entry_set_mtime(ae, 123456789, 0);
//     archive_write_header(a, entry);
//     archive_write_data(a, "abcde", 5); // Note 3
//     archive_write_finish_entry(a);
//     archive_write_free(a);
//     archive_entry_free(entry);
// }

// static void extract2(const char *filename)
// {
//     struct archive *a;
//     struct archive *ext;
//     struct archive_entry *entry;
//     int r;

//     a = archive_read_new();
//     archive_read_support_filter_all(a);
//     archive_read_support_format_all(a);
    
//     ext = archive_write_disk_new();
//     archive_write_disk_set_standard_lookup(ext);



//     r = archive_read_open_filename(a, filename, 10240); // Note 1
//     if (r != ARCHIVE_OK) {
//         exit(1);
//     }

//     while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
//         if (r == ARCHIVE_EOF) {
//             break;
//         }
//         if (r < ARCHIVE_OK) {
//             std::cout << archive_error_string(a) << std::endl;
//         }
//         if (r < ARCHIVE_WARN) {
//             exit(1);
//         }
//         if(hasEnding(archive_entry_pathname(entry))) {
//             r = archive_write_header(ext, entry);
//             const char* path = archive_entry_pathname( entry );
//             // boost::filesystem::path p(path);
//             // //archive_entry_set_pathname(entry, ("/backgrounds/" + ((p.filename()).string()).c_str()));

//             // char b;
//             // std::string text = std::string("/backgrounds/") + ((p.filename()).string());
//             // const char* newPath = text.c_str();
//             // archive_entry_set_pathname(entry, newPath);

//             //then use text.c_str() if you need const char*


//             // std::cout << (((p.filename()).string()).c_str()) << std::endl;
//             if (r < ARCHIVE_OK) {
//                 std::cout << archive_error_string(ext);
//             } else if (archive_entry_size(entry) > 0) {
//                 r = copy_data(a, ext);
//             }
//             if (r < ARCHIVE_OK) {
//                 std::cout << archive_error_string(ext);
//             }
//             if (r < ARCHIVE_WARN) {
//                 exit(1);
//             }
//             r = archive_write_finish_entry(ext);

//             if (r < ARCHIVE_OK) {
//                 fprintf(stderr, "%s\n", archive_error_string(ext));
//             }
//             if (r < ARCHIVE_WARN) {
//                 exit(1);
//             }
//             std::cout << archive_entry_pathname(entry) << std::endl;
//         }
//         archive_read_data_skip(a);
//     }

//     r = archive_read_free(a);

//     if (r != ARCHIVE_OK) {
//         exit(1);
//     }
//     archive_read_close(a);
//     archive_read_free(a);
//     archive_write_close(ext);
//     archive_write_free(ext);
//     exit(0);
// }



#include <semantics/validate/validate-internal.h>

struct yfs_symtab * current_scope;

int find_symbol(
    struct yf_sym ** sym, struct yfs_symtab * symtab,
    char * name
) {
    int depth = 0;
    while (symtab != NULL) {
        if ( (*sym = yfh_get(symtab->table, name)) != NULL) {
            return depth;
        }
        depth++;
        symtab = symtab->parent;
    }
    return -1;
}

int enter_scope(struct yfs_symtab ** stuff) {

    struct yfs_symtab * old_symtab, * new_symtab;
    old_symtab = current_scope;

    new_symtab = yf_malloc(sizeof (struct yfs_symtab));
    if (!new_symtab) {
        return 1;
    }
    new_symtab->table = yfh_new();
    if (!new_symtab->table) {
        free(new_symtab);
        return 1;
    }

    new_symtab->parent = old_symtab;
    current_scope = new_symtab;

    if (stuff)
        *stuff = new_symtab;
    
    return 0;

}

void exit_scope(void) {
    /* Don't free scope - used later during codegen */
    current_scope = current_scope->parent;
}

int yfv_add_type(
    struct yf_file_compilation_data * fdata,
    struct yfs_type * type
) {
    /**
     * Set a value in the hashmap.
     */
    return yfh_set(fdata->types.table, type->name, type);
}

struct yfs_type * yfv_get_type_t(
    struct yf_file_compilation_data * fdata,
    struct yfcs_type type
) {
    /**
     * Get a value from the hashmap.
     */
    return yfv_get_type_s(fdata, type.databuf);
}

struct yfs_type * yfv_get_type_s(
    struct yf_file_compilation_data * fdata,
    char * typestr
) {
    /**
     * Get a value from the hashmap.
     */
    return yfh_get(fdata->types.table, typestr);
}
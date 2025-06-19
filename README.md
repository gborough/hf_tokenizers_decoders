# Hugging Face Tokenizers Decoders

This is a C implementation of the Hugging Face Tokenizers' decoders, which is the last step of the Tokenizers pipeline.

It's an experiment to explore the technical feasibility of making a library that improves interoperability for other languages like OCaml to use C bindings. Use at your own risk(I doubt it's useful at all at the current stage), but if you are compelled to use it, it should be safe to use. Should you find any security issues, please let me know.

All naming conventions follow closely to the original Rust implementation, with small but understandable variations idiomatic to C.

## Usage

1. Assuming you have a folder named `vendors` at project root, run: git submodule add https://github.com/gborough/hf_tokenizers_decoders.git vendors/hf_tokenizers_decoders.

2. Assuming you are using Makefile, you should have a step that runs the following or similar:

```
@if [ ! -d vendors/hf_tokenizers_decoders/.build ]; then \
    echo ">> Initializing hf_tokenizers_decoders submodule..."; \
    git submodule update --init --recursive vendors/hf_tokenizers_decoders; \
fi
@echo ">> Building hf_tokenizers_decoders..."
$(MAKE) -C vendors/hf_tokenizers_decoders
```

3. Include the "decoders.h" header and linker flag when compiling: 
```
CFLAGS = ... -Ivendors/hf_tokenizers_decoders/include
LDFLAGS = ... -Lvendors/hf_tokenizers_decoders/.build/decoders.so
```

## Type explanations and examples

All string type char* is wrapped in cstr type, which is re-exported from stclib for easier management.

The vector of string type is named string_vec, wrapped in stclib vector type.

See include/utils for all other wrapper types.

Inside tests folder, see all test_decoder_*.c files for actual usage.

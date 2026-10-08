// SPDX-FileCopyrightText: Copyright 2021 yuzu Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <shader_compiler/common/bit_field.h>
#include <shader_compiler/common/common_types.h>
#include <shader_compiler/frontend/maxwell/translate/impl/impl.h>
#include <shader_compiler/common/log.h>

namespace Shader::Maxwell {
namespace {
enum class Mode : u64 {
    Default,
    Patch,
    Prim,
    Attr,
};

enum class Shift : u64 {
    Default,
    U16,
    B32,
};

} // Anonymous namespace

void TranslatorVisitor::ISBERD(u64 insn) {
    union {
        u64 raw;
        BitField<0, 8, IR::Reg> dest_reg;
        BitField<8, 8, IR::Reg> src_reg;
        BitField<31, 1, u64> skew;
        BitField<32, 1, u64> o;
        BitField<33, 2, Mode> mode;
        BitField<47, 2, Shift> shift;
    } const isberd{insn};

    if (isberd.skew != 0) {
        throw NotImplementedException("SKEW");
    }
    if (isberd.o != 0) {
        throw NotImplementedException("O");
    }
    if (isberd.mode != Mode::Default) {
        throw NotImplementedException("Mode {}", isberd.mode.Value());
    }
    if (isberd.shift != Shift::Default) {
        throw NotImplementedException("Shift {}", isberd.shift.Value());
    }
    // Diagnostics: disambiguate this site from the other "(STUBBED) called"
    // emitters (VOTE_vtg, EmitInvocationInfo). Behavior is unchanged: with
    // all-default parameters ISBERD is stubbed as an identity register copy,
    // which is only correct when the internal stage buffer is not in use
    // (vertex-only programs); tessellation/geometry reads return wrong data.
    LOG_WARNING(Shader, "ISBERD: internal stage buffer entry read stubbed as identity copy (internal stage buffer contents ignored)");
    X(isberd.dest_reg, X(isberd.src_reg));
}

} // namespace Shader::Maxwell

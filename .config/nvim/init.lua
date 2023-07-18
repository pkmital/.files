vim.cmd([[
set runtimepath^=~/.vim runtimepath+=~/.vim/after
let &packpath = &runtimepath

source ~/.vimrc
]])

require('packer').startup(function()
    -- Packer can manage itself
    use 'wbthomason/packer.nvim'

    use {'vim-airline/vim-airline', 'vim-airline/vim-airline-themes'}

    -- Telescope settings (replacing ctrlp.vim)
    use {
        'nvim-telescope/telescope.nvim',
        requires = {
            {'nvim-lua/plenary.nvim'},
            {'nvim-telescope/telescope-fzf-native.nvim', run = 'make' }
        }
    }

    use 'tpope/vim-surround'
    use {'neoclide/coc.nvim', branch = 'release'}
    use 'tpope/vim-commentary'
end)



require("indent_blankline").setup {
    -- for example, context is off by default, use this to turn it on
    show_current_context = true,
    show_current_context_start = true,
}

local builtin = require('telescope.builtin')
vim.keymap.set('n', '<leader>ff', builtin.find_files, {})
vim.keymap.set('n', '<leader>fg', builtin.live_grep, {})
vim.keymap.set('n', '<leader>fb', builtin.buffers, {})
vim.keymap.set('n', '<leader>fh', builtin.help_tags, {})

-- Coc settings (replacing YouCompleteMe)
vim.g.coc_global_extensions = {
    'coc-json',
    'coc-tsserver',
    'coc-python',
    'coc-snippets',
}

-- Airline settings
vim.g.airline_powerline_fonts = 1
vim.g.airline_theme='dark'
vim.g['airline#extensions#tabline#enabled'] = 1
vim.g['airline#extensions#tabline#formatter'] = 'unique_tail'


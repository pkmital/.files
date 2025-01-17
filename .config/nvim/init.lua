local vim = vim
local opt = vim.opt

vim.cmd([[
  set runtimepath^=~/.vim runtimepath+=~/.vim/after
  let &packpath = &runtimepath
  source ~/.vimrc
]])

require('packer').startup(function()
    -- Packer can manage itself
    use 'wbthomason/packer.nvim'

    -- use {'vim-airline/vim-airline', 'vim-airline/vim-airline-themes'}
    use {
	  'nvim-lualine/lualine.nvim',
	  requires = { 'nvim-tree/nvim-web-devicons', opt = true }
	}

    -- Telescope settings (replacing ctrlp.vim)
    use {
        'nvim-telescope/telescope.nvim',
        requires = {
            {'nvim-tree/nvim-web-devicons'},
            {'nvim-lua/plenary.nvim'},
            {'nvim-telescope/telescope-fzf-native.nvim', run = 'make' }
        }
    }

    use 'tpope/vim-surround'
    use {'neoclide/coc.nvim', branch = 'release'}
    use {'yaegassy/coc-ruff', run = 'yarn install --frozen-lockfile'}
    use 'tpope/vim-commentary'

    -- colorscheme
    use {"adisen99/apprentice.nvim", requires = {"rktjmp/lush.nvim"}}

    -- Copilot
    use 'github/copilot.vim'

    -- Better Folds / nvim-ufo
    use {'kevinhwang91/nvim-ufo', requires = 'kevinhwang91/promise-async'}

    -- GitSigns
    use 'lewis6991/gitsigns.nvim'

    -- TZ True Zen Mode
    use({"Pocco81/true-zen.nvim",
      config = function()
         require("true-zen").setup {
          -- your config goes here
          -- or just leave it empty :)
         }
      end,
    })

    -- Nicer error symbols
    use "folke/trouble.nvim"

    -- Kanagawa colorscheme
    use 'rebelot/kanagawa.nvim'
end)

vim.cmd("colorscheme kanagawa-dragon")

-- Folding / nvim-ufo
vim.keymap.set('n', 'zR', require('ufo').openAllFolds)
vim.keymap.set('n', 'zM', require('ufo').closeAllFolds)
require('ufo').setup()

-- Treesitter
require'nvim-treesitter.configs'.setup {
  -- A list of parser names, or "all" (the five listed parsers should always be installed)
  ensure_installed = { "c", "lua", "vim", "vimdoc", "query", "python" },

  -- Install parsers synchronously (only applied to `ensure_installed`)
  sync_install = false,

  -- Automatically install missing parsers when entering buffer
  -- Recommendation: set to false if you don't have `tree-sitter` CLI installed locally
  auto_install = true,

  -- highlight = { enable = true },
}
-- Treesitter for folding
opt.foldmethod = "expr"
opt.foldexpr = "nvim_treesitter#foldexpr()"
-- vim.api.nvim_create_autocmd({'BufEnter','BufAdd','BufNew','BufNewFile','BufWinEnter'}, {
--   group = vim.api.nvim_create_augroup('TS_FOLD_WORKAROUND', {}),
--   callback = function()
--     vim.opt.foldmethod     = 'expr'
--     vim.opt.foldexpr       = 'nvim_treesitter#foldexpr()'
--   end
-- })


-- Setup Telescope mappings
-- More here: https://github.com/nvim-telescope/telescope.nvim#pickers
local builtin = require('telescope.builtin')
vim.keymap.set('n', '<leader>ff', builtin.find_files, {})
vim.keymap.set('n', '<C-p>', builtin.find_files, {})
vim.keymap.set('n', '<leader>fg', builtin.live_grep, {})
vim.keymap.set('n', '<leader>fb', builtin.buffers, {})
vim.keymap.set('n', '<leader>fh', builtin.help_tags, {})
vim.keymap.set('n', '<leader>ft', builtin.treesitter, {})

-- -- For coloscheme
-- -- Load and setup function to choose plugin and language highlights
require('lualine').setup({
  options = {
    icons_enabled = true,
  }
})
-- vim.o.background = "dark" -- or "light" for light mode
require('lush')(require('apprentice').setup({
  plugins = {
    "buftabline",
    "coc",
    "cmp", -- nvim-cmp
    "fzf",
    "gitgutter",
    "gitsigns",
    "lsp",
    "lspsaga",
    "nerdtree",
    "netrw",
    "nvimtree",
    "neogit",
    "packer",
    "signify",
    "startify",
    "syntastic",
    "telescope",
    "treesitter"
  },
  langs = {
    "c",
    "clojure",
    "coffeescript",
    "csharp",
    "css",
    "elixir",
    "golang",
    "haskell",
    "html",
    "java",
    "js",
    "json",
    "jsx",
    "lua",
    "markdown",
    "moonscript",
    "objc",
    "ocaml",
    "purescript",
    "python",
    "ruby",
    "rust",
    "scala",
    "typescript",
    "viml",
    "xml"
  }
}))
vim.g.apprentice_contrast_dark = "dark"
vim.g.apprentice_hls_lspreference = "bright_yellow"
vim.g.apprentice_hls_cursor = "bright_yellow"
vim.g.apprentice_hls_highlight = "bright_yellow"
vim.g.apprentice_italicize_booleans = true
vim.g.apprentice_tabline_sel = "fg0"

require("indent_blankline").setup {
    -- for example, context is off by default, use this to turn it on
    show_current_context = true,
    show_current_context_start = true,
}

-- Coc settings (replacing YouCompleteMe)
vim.g.coc_global_extensions = {
    'coc-json',
    'coc-tsserver',
    'coc-pyright',
    'coc-snippets',
    'coc-prettier',
    'coc-pairs'
}

-- Airline settings
vim.g.airline_powerline_fonts = 1
vim.g.airline_theme='dark'
vim.g['airline#extensions#tabline#enabled'] = 1
vim.g['airline#extensions#tabline#formatter'] = 'unique_tail'

require "git-config"

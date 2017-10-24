execute pathogen#infect()
execute pathogen#helptags()

colorscheme apprentice
let mapleader = ","

" Match parens
set showmatch
hi MatchParen cterm=none ctermbg=green ctermfg=blue

" Shared X11 Clipboard
if has('unix')
  if has('mac')       " osx
    set clipboard=unnamed
  else                " linux, bsd, etc
    set clipboard=unnamedplus
  endif
endif

" add ctrlp.vim setting
let g:ctrlp_cache_dir = $HOME . '/.cache/ctrlp'
if executable('ag')
      let g:ctrlp_user_command = 'ag %s -l --nocolor -g ""'
  endif
set wildignore+=*/tmp/*,*.so,*.swp,*.zip,*/vendor/*,*/\.git/*
let g:ctrlp_custom_ignore = 'tmp$\|\.git$\|\.hg$\|\.svn$\|.rvm$|.bundle$\|vendor'
let g:ctrlp_working_path_mode = 0
let g:ctrlp_match_window_bottom=1
let g:ctrlp_max_height=25
let g:ctrlp_match_window_reversed=0
let g:ctrlp_mruf_max=500
let g:ctrlp_follow_symlinks=1
let g:ctrlp_clear_cache_on_exit=0
"
" Use py matcher for performance
let g:ctrlp_match_func = { 'match': 'pymatcher#PyMatch' }

" ctrl-p funky for functions
nnoremap <Leader>q :CtrlPFunky<Cr>
" narrow the list down with a word under cursor
nnoremap <Leader>Q :execute 'CtrlPFunky ' . expand('<cword>')<Cr>

" PyDocString
let g:pydocstring_templates_dir = '~/.vim/bundle/vim-pydocstring/test/templates/numpy'
nmap <silent> <C-_> <Plug>(pydocstring)

" Buffer back/forward like C-o and C-I
nmap <leader>r :bp<CR>
nmap <leader>f :bn<CR>

" GFM
let g:vim_markdown_preview_browser='Google Chrome'
let g:vim_markdown_preview_github=1

" Undo
if has("persistent_undo")
    set undodir=~/.undodir/
    set undofile
endif

" ,y and ,p for cross-vim clipboard w/o X11
vmap <leader>y :w! /tmp/vitmp<CR>                                                                   
nmap <leader>p :r! cat /tmp/vitmp<CR>

" Create session, continue w/ vim -S
nnoremap <leader>s :mksession<CR>

" NERDTree
nnoremap <leader>k :NERDTreeToggle<CR>
let NERDTreeIgnore = ['\.pyc$', '\.ipynb$', '\.png$', '\.jpeg$', '\.jpg$', '\.JPEG$', '\.JPG$', '\.pkl$', '\.ckpt$']

" TagBar
nnoremap <F8> :TagbarToggle<CR>
let g:tagbar_compact = 1
let g:tagbar_show_linenumbers = 1
let g:tagbar_autopreview = 0
let g:tagbar_sort = 0
let g:tagbar_autofocus = 1
let g:tagbar_autoclose = 1
set previewheight=25

" " PyMode PEP 8
" nnoremap <leader>l :PymodeLint<CR>
" nnoremap <leader>L :PymodeLintAuto<CR>

" Folding for Python; should move this to ftplugin
nnoremap <space> za
vnoremap <space> zf

" YAPF
nnoremap <leader>L :call Yapf("--style google")<CR>

" Line Numbers
set nu

"  Yank/Paste clipboard
nnoremap <C-y> "+y
vnoremap <C-y> "+y
nnoremap <C-p> "+gP
vnoremap <C-p> "+gP
set pastetoggle=<F2>

" Move to normal mode when using arrows
inoremap <silent> <Up> <ESC><Up>
inoremap <silent> <Down> <ESC><Down>

" Make jj ESC
inoremap jj <Esc>

" Show gutter always, for Git
" let g:gitgutter_override_sign_column_highlight = 0
" let g:gitgutter_sign_column_always = 0

" Toggle gutter shit
nnoremap <F5> <esc>:SignifyDisable<CR>:set nonumber<CR>
nnoremap <F6> <esc>:SignifyDisable<CR>:set number<CR>
nnoremap <F5> <esc>:SignifyDisable<CR>:set nonumber<CR>
nnoremap <F6> <esc>:SignifyDisable<CR>:set number<CR>

" Tab/Indents
set autoindent
set ts=4
set expandtab
set shiftwidth=4
"set cursorline

" Clear highlighting on ESC in normal
nnoremap <esc> :noh<return>:SyntasticReset<return><esc>
nnoremap <esc>^[ <esc>^[

" Seems criminal but...
" set mouse=a

" Vim Slimeness
let g:slime_target = "tmux"
let g:slime_python_ipython = 1

" Airline
let g:airline_theme='molokai'
let g:airline_left_sep=''
let g:airline_right_sep=''

" Vim Markdown conceals stuff. Don't.  Also don't fold.
" set nofoldenable
let g:vim_markdown_conceal = 0
let g:vim_markdown_folding_disabled = 1
let g:vim_markdown_fenced_languages = ['bash=sh', 'shell=sh', 'c++=cpp', 'viml=vim', 'ini=dosini']

" Pandoc
" let g:pandoc#syntax#conceal#use = 0

" Enable spell checking for markdown files
au BufRead *.md setlocal spell
au BufRead *.mdown setlocal spell

" Python highlighting
let g:python_highlight_all = 1
let g:pymode_syntax = 1
let g:pymode_syntax_all = 1
let g:pymode_syntax_indent_errors = g:pymode_syntax_all
let g:pymode_syntax_space_errors = g:pymode_syntax_all
let g:pymode_rope = 0
let g:pymode_lint_signs = 1

" Goyo Distraction free editing
let g:goyo_height = '100%'
let g:goyo_width = '90%'
let g:goyo_pads = 0
nnoremap <Leader>z :Goyo<CR>i<Esc>`^

" Documentation
"let g:pymode_doc = 1
let g:pymode_doc_key = 'K'

" Linting
" let g:pymode_lint = 1
" let g:pymode_lint_checker = "pyflakes,pep8"
" let g:pymode_options_max_line_length = 90
" Auto check on save
"let g:pymode_lint_write = 1


" Syntax
filetype on
syntax on
set ignorecase
set smartcase

" Search highlight/Incremental
set hlsearch
set incsearch
set modelines=0
set wildmenu
set wildmode=longest:full

filetype plugin indent on

set fillchars=vert:│    " that's a vertical box-drawing character
" UTF-8
set encoding=utf-8

" Syntastic Error/Warn
let g:syntastic_python_checkers = ['python', 'pyflakes', 'flake8']
set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*
let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0
nnoremap <leader>r :SyntasticReset<CR>

" Javascript
let g:javascript_plugin_jsdoc = 1
let g:jsx_ext_required = 0

" Markdown Ctag
let g:tagbar_type_markdown = {
            \ 'ctagstype' : 'markdown',
            \ 'kinds' : [
                \ 'h:headings',
                \ 'l:links',
                \ 'i:images'
            \ ],
    \ "sort" : 0
\ }

" UndoTree
nnoremap <leader>u <ESC>:UndotreeToggle<cr>

let g:lightline = {
      \ 'colorscheme': 'jellybeans',
      \ }
"" tabs
"let g:airline#extensions#tabline#enabled = 1
"let g:airline#extensions#tabline#left_sep = ' '
"let g:airline#extensions#tabline#left_alt_sep = '|'
"let g:airline#extensions#tabline#right_alt_sep = ''
"
"" configure the minimum number of tabs needed to show the tabline.
"let g:airline#extensions#tabline#tab_min_count = 2
"let g:airline#extensions#tabline#show_buffers = 0
"
"" tab shortcuts
"nnoremap <C-b>  :tabprevious<CR>
"inoremap <C-b>  <Esc>:tabprevious<CR>i
"nnoremap <C-m>  :tabnext<CR>
"inoremap <C-m>  <Esc>:tabnext<CR>i
"nnoremap <C-t>  :tabnew<CR>
"inoremap <C-t>  <Esc>:tabnew<CR>i
"nnoremap <C-x>  :tabclose<CR>
"inoremap <C-x>  <Esc>:tabclose<CR>i

" Easier Pane Navigation
noremap <C-j> <C-W>j
noremap <C-k> <C-W>k
noremap <C-h> <C-W>h
noremap <C-l> <C-W>l

" Moving around in wrapped lines:
autocmd FileType html,markdown,text nnoremap <expr> j v:count ? 'j' : 'gj'
autocmd FileType html,markdown,text nnoremap <expr> k v:count ? 'k' : 'gk'
autocmd FileType html,markdown,text vnoremap <expr> j v:count ? 'j' : 'gj'
autocmd FileType html,markdown,text vnoremap <expr> k v:count ? 'k' : 'gk'

" YouCompleteMe
let g:ycm_python_binary_path = '/opt/conda/bin/python'
nnoremap <leader>g <ESC>:YcmCompleter GoTo<CR>

" Redraw hacks to force refresh
set ttyfast
set lazyredraw
" These didn't seem to help w/ my issues w/ markdown + tmux
" au FileWritePost * :redraw!
" au TermResponse * :redraw!
" au TextChanged * :redraw!
" au QuickFixCmdPre * :redraw!
" au QuickFixCmdPost * :redraw!

" Grammarous
nnoremap <leader>z <ESC>:GrammarousCheck<CR>

" Last place editing
if has("autocmd")
  au BufReadPost * if line("'\"") > 0 && line("'\"") <= line("$")
    \| exe "normal! g'\"" | endif
endif

" Retab 2 -> 4
nnoremap <leader>t <ESC>:set ts=2 noet | retab! | set et ts=4 | retab

" Turn off automatic visual selection w/ mouse
set mouse-=a

" Delays
set timeoutlen=1000 ttimeoutlen=0

" Tired of lowercasing stuff by mistake
vnoremap u :noh<cr>

" Use silver surfer for grep
if executable('ag')
  " Use ag over grep
  set grepprg=ag\ --nogroup\ --nocolor

  " Use ag in CtrlP for listing files. Lightning fast and respects .gitignore
  let g:ctrlp_user_command = 'ag %s -l --nocolor -g ""'

  " ag is fast enough that CtrlP doesn't need to cache
  let g:ctrlp_use_caching = 0
endif

" bind K to grep word under cursor
nnoremap K :grep! "\b<C-R><C-W>\b"<CR>:cw<CR>

" bind \ (backward slash) to grep shortcut
command -nargs=+ -complete=file -bar Ag silent! grep! <args>|cwindow|redraw!

nnoremap \ :Ag<SPACE>

